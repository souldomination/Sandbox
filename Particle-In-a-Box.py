import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
 
fig, ax = plt.subplots()
 
 
class Particle(object):
    def __init__(self, init_pos, init_vel):
        self.pos = np.array(init_pos)
        self.vel = np.array(init_vel)
 
 
def OverlapWithOthers(particles, pos, radius):
    for particle in particles:
        if np.linalg.norm(particle.pos-pos) <= 2*radius:
            return True
    return False
 
 
class Box(object):
    def __init__(self, width=3, height=3, n=16, radius=0.05):
        self.width = width
        self.height = height
 
        self.border = plt.Rectangle((0, 0), width, height,
                                    linewidth=2, fill=False)
 
        self.n = n
        self.radius = radius
 
        # make particles with random position and velocity
        self.particles = []
        for i in range(n):
            while True:
                pos = ((width-2*self.radius)*np.random.random()+self.radius,
                       (height-2*self.radius)*np.random.random()+self.radius)
                if not OverlapWithOthers(self.particles, pos, self.radius):
                    break
 
            vel = (2*np.random.random()-1, 2*np.random.random()-1)
            self.particles.append(Particle(pos, vel))
 
        self.time = 0
 
    def step(self, dt):
        # wall-particle collision
        for particle in self.particles:
            if particle.pos[0] <= self.radius or \
                    particle.pos[0] >= self.width - self.radius:
                particle.vel[0] *= -1
            if particle.pos[1] <= self.radius or \
                    particle.pos[1] >= self.height - self.radius:
                particle.vel[1] *= -1
 
        # particle-particle collision
        for i in range(self.n):
            for j in range(i+1, self.n):
                p1 = self.particles[i]
                p2 = self.particles[j]
 
                displacement = p1.pos - p2.pos
                dist = np.linalg.norm(displacement)
 
                if dist <= 2*self.radius:
                    n = displacement/dist
                    vel1prime = p1.vel - np.inner(p1.vel-p2.vel, n)*n
                    vel2prime = p2.vel - np.inner(p2.vel-p1.vel, n)*n
 
                    p1.vel = vel1prime
                    p2.vel = vel2prime
 
        # position update
        for i in range(self.n):
            self.particles[i].pos += self.particles[i].vel * dt
 
        self.time += dt
 
 
box = Box()
 
ax.set(xlim=[-0.5, box.width+0.5], ylim=[-0.5, box.height+0.5])
ax.set_aspect('equal', adjustable='box')
 
ax.add_patch(box.border)
 
circles = [plt.Circle((0, 0), box.radius, color='r') for i in range(box.n)]
time_text = ax.text(0, -0.2, '')
 
 
def animate(x):
    box.step(0.02)
 
    for i in range(box.n):
        circles[i].center = tuple(box.particles[i].pos)
        ax.add_artist(circles[i])
 
    time_text.set_text('time = {:.1f}'.format(box.time))
 
    return (*circles, time_text)
 
 
anim = animation.FuncAnimation(fig, animate,
                               frames=1001, interval=20,
                               repeat=False, blit=True)
 
#anim.save('Particles In a Box.mp4', fps=50, extra_args=['-vcodec', 'libx264'])
