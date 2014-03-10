import subprocess

STEPS = 24 * 60

X = (-0.763, -0.763043)
Y = (0.096, 0.096087)
D = (2.0, 0.0003)
H = (0, 100)

def tween(step, (s, e)):
    return (e - s) / (STEPS-1) * step + s

def diam(step, (s, e)):
    return e + abs(tween(step, (s, e)) *
        ( 1 - pow(abs(e - s), 1.0 / STEPS) ** (STEPS - step) ))

for s in range(0, STEPS):
    subprocess.call(['dist/bin/mandelbrot', 'out%04d.tga' % (s+1), '1920', '1080',
        '-cx', str(tween(s, X)), '-cy', str(tween(s, Y)), '-d', str(diam(s, D)),
        '-h', str(tween(s, H))])
