import matplotlib.pyplot as plt
import numpy as np


# Down
scale_up = 0.01
scaleIncrement_up = 0.0001
scale_arr_up = np.zeros(1000)

scale_down = 0.1
scaleIncrement_down = 0.001
scale_arr_down = np.zeros(1000)

loop_arr = np.zeros(1000)


for i in range(1000):
    scale_up = scale_up + scaleIncrement_up
    scaleIncrement_up *= 1.01
    scale_arr_up[i] = scale_up

    scale_down = scale_down - scaleIncrement_down
    scaleIncrement_down *= 0.99
    scale_arr_down[i] = scale_down

    loop_arr[i]=i


plt.figure()
plt.plot(loop_arr, scale_arr_up)
plt.show()

plt.figure()
plt.plot(loop_arr, scale_arr_down)
plt.show()