from hoomd import *
from hoomd import md
from hoomd import deprecated
from hoomd import correlator
import math as m
import numpy as np
import sys
context.initialize('--mode=cpu')

def fcc():
    ############################## Define Variables ###############################
    sigma_cc = 2.0
    epsilon_cc = 2.0
    sigma_pc = 1.0
    epsilon_pc = 50.0 # number of "sticky ends"

    R = 10.00 # 11.1 nm inner radius of the colloid
    R_true = (R + 1.12 * sigma_pc) # hard sphere radius of the colloid
    R_polymer = 0.5 # radius of the polymer to set Delta to 0 in FENE
    D = 2*(R+2.5*sigma_pc)-1.12*2.0
    spacing = D + 3 #space out the colloids a little (nearest neighbor spacing)
    a = np.sqrt(2)*spacing #lattice parameter
    pos_offset = np.multiply([1,1,1],(R_true*np.sqrt(2)/2))  ###possible values of offset that defines polymer position. will be used to check for periodic bound

    eql_steps = 1e4
    run_steps = 1e5
    dump_period = run_steps/10

    T = 1.0
    N_Kuhn = 60 # number of Kuhn steps per polymer chain
    Hd = 3*T/(N_Kuhn) # spring constant


    ############################### Define Functions ###############################
    ### find distance between two particles, accounting for periodic box conditions ###
    def dist(bead1, bead2):
        dist=[0,0,0]
        for i in np.arange(3):
            dist[i] = abs(bead1[i]-bead2[i])
            if dist[i] >= box_length/2:
                dist[i] -= box_length
        return np.linalg.norm(dist)

    ## find the instantaneous bond stress
    def stressxy(timestep):
        total = 0
        for i in np.arange(N_bonds):
            a,b = bond_index[i]
            a = int(a)
            b = int(b)
            apos = system.particles[a].position[0]
            bpos = system.particles[b].position[0]
            distance = apos-bpos
            force = harmonic.forces[b].force[1]     ##y-component of force in direction toward particle a
            total = total + distance*force
        stress = total/box_volume

        return stress

    ### correct for period box conditions when placing polymers on colloids
    def position_correction(x, y):
        for i in np.arange(3):
            if x[i] >= box_length/2:
                x[i] -= box_length
            if y[i] <= -1*box_length/2:
                y[i] += box_length
        return x, y

    ############################## Initialize System ###############################
    system = init.create_lattice(lattice.fcc(a, type_name = 'colloid'), 2)
    snapshot = system.take_snapshot(bonds=True)
    N_colloid = len(snapshot.particles.diameter)
    N_polymer = 12  					 # number of nearest neighbors
    N_bonds = int((N_colloid*N_polymer)/2)
    N_tot = N_colloid*N_polymer + N_colloid
    box_length = snapshot.box.Lx
    box_volume = snapshot.box.get_volume()
    snapshot.particles.resize(N_tot)
    snapshot.particles.types = ['colloid', 'polymer']
    snapshot.bonds.types = ['polymerbond']
    snapshot.particles.typeid[:] = [0]*N_colloid + [1]*N_polymer*N_colloid
    snapshot.particles.diameter[:]=[2*R_true]*N_colloid+[2*R_polymer]*N_polymer*N_colloid
    snapshot.bonds.resize(int(N_colloid*N_polymer/2))
    print('BOX LENGTH = ', snapshot.box.Lx)

    ## place polymers ##
    for i in np.arange(N_colloid):
        center = snapshot.particles.position[i]
        pos = center+pos_offset
        neg = center-pos_offset
        pos, neg = position_correction(pos, neg)

        snapshot.particles.position[i+N_colloid*1] = [pos[0], pos[1], center[2]] 	##x-y plane attachment
        snapshot.particles.position[i+N_colloid*2] = [pos[0], neg[1], center[2]]
        snapshot.particles.position[i+N_colloid*3] = [neg[0], pos[1], center[2]]
        snapshot.particles.position[i+N_colloid*4] = [neg[0], neg[1], center[2]]

        snapshot.particles.position[i+N_colloid*5] = [center[0], pos[1], pos[2]]	##y-z plane attachment
        snapshot.particles.position[i+N_colloid*6] = [center[0], pos[1], neg[2]]
        snapshot.particles.position[i+N_colloid*7] = [center[0], neg[1], pos[2]]
        snapshot.particles.position[i+N_colloid*8] = [center[0], neg[1], neg[2]]

        snapshot.particles.position[i+N_colloid*9] = [pos[0], center[1], pos[2]]   ##x-z plane attachment
        snapshot.particles.position[i+N_colloid*10] = [pos[0], center[1], neg[2]]
        snapshot.particles.position[i+N_colloid*11] = [neg[0], center[1], pos[2]]
        snapshot.particles.position[i+N_colloid*12] = [neg[0], center[1], neg[2]]

    ## define bonds ##
    k=0
    for i in np.arange(N_colloid, N_tot):
        for j in np.arange(i+1, N_tot):
            # ADJUST FOR PERIODIC BOUNDARY
            delta = dist(bead1 = snapshot.particles.position[i], bead2 = snapshot.particles.position[j])
            # delta = np.linalg.norm(snapshot.particles.position[i]-snapshot.particles.position[j])
            if round(delta, 2) <= round(spacing-2*R_true,2):
                snapshot.bonds.group[k] = [i,j]
                k=k+1

    bond_index = snapshot.bonds.group   ##define bond index for callback function. Do not have to access snapshot everytime function called
    system.restore_snapshot(snapshot)


    ########################### Add Potentials and Bonds ###########################

    ### shifted lennard jones ###
    # polymer - colloid associative potential
    def slj_pc(r, rmin, rmax, delta, epsilon, sigma):
        V = 4 * epsilon * ((sigma / (r-delta))**12 - (sigma/(r-delta))**6)
        F = 4 * epsilon / (r-delta) * ( 12 * (sigma / (r-delta))**12 - 6 * (sigma / (r-delta))**6)
        return (V, F)

    ### colloid - colloid hard sphere, purely repulsive potential ###
    def slj_cc(r, rmin, rmax, delta, epsilon, sigma):
        V = 4 * epsilon * ( (sigma / (r-delta))** 12 - (sigma / (r-delta))**6 + 1/4)
        F = 4 * epsilon / (r-delta) * (12 * (sigma / (r-delta))**12 - 6 * (sigma / (r-delta))**6)
        return (V, F)

    #### FENE Bonds ###
    #fene = md.bond.fene()
    #fene.bond_coeff.set('polymerbond', k=Hd, r0=N_Kuhn, epsilon = 0.0, sigma = 1.0)

    ##Harmonic Bond##
    harmonic = md.bond.harmonic()
    harmonic.bond_coeff.set('polymerbond', k=Hd, r0=0)

    nl = md.nlist.tree()
    table = md.pair.table(width = int(1e5), nlist = nl)
    table.pair_coeff.set('colloid','polymer', func = slj_pc, rmin= 0, rmax = R+2.5*sigma_pc,
        coeff = dict(delta = R, epsilon = epsilon_pc, sigma = sigma_pc))
    table.pair_coeff.set('colloid','colloid', func = slj_cc, rmin= 0 , rmax = D+1.12*sigma_cc,
        coeff = dict(delta= D, epsilon = epsilon_cc, sigma = sigma_cc))
    table.pair_coeff.set('polymer','polymer', func = slj_pc, rmin= 0 , rmax = 5,
        coeff = dict(delta= R, epsilon = 0.0, sigma = 1.0))


    ############################### Define Integrator ##############################

    # integrate_all= md.integrate.brownian(group=group.all(), kT=T, seed=2094)
    integrate_all = md.integrate.langevin(group=group.all(), kT=T, seed = 204)
    integrate_all.set_gamma('colloid',100.0)
    integrate_all.set_gamma_r('colloid',100.0)
    integrate_all.set_gamma('polymer',1.0)
    integrate_all.set_gamma_r('polymer',1.0)
    integrator_mode = md.integrate.mode_standard(dt=0.005)
    run(eql_steps)

    # dump pressure values
    logger = analyze.log(filename = 'pressure_xy.log', quantities = ['pressure_xy'], period = 10, overwrite = True)
    # logger = analyze.log(filename = 'stress_xy.log', quantities = ['stress_xy'], period = 10, overwrite = True)
    # logger.register_callback('stress_xy', stressxy)

    corr = correlator.correlate.correlate(filename="corr.log", quantities=['pressure_xy'], period=10, phase=0)


    #dump to make job restartable
    #dump.gsd(filename="restart.gsd", group=group.all(), truncate=True, period=10000, phase=0)

    ### dump files for VMD ###
    #dcd = dump.dcd(filename="dump.dcd", overwrite = True, period=100)
    #xml = deprecated.dump.xml(group = group.all(), filename= 'init.xml', all = True)

