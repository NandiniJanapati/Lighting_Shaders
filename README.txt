This is the fourth assignment I did for my Computer Graphics class at Texas A&M Univeristy.
The goal was to create different shaders in GLSL to rasterize the same bunny.obj with OpenGL.

------
There 3 separate shader programs that can be switched between using '0', '1', '2'
    0: Uses the Gouraud shading approach
    1: Uses the Phong shading approach
    2: Uses the silouhette shader (every fragment is black except for the ones where the angle between the normal and the eye is about 90 degrees)

There are 3 materials you can switch between with preset kd, ks, ka, and s data using 'm' and 'M' (press m to cycle forward, M to cycle back)
There are 2 lights in the scene with preset colors. You can cycle between them using 'l' and 'L' (press l to cycle forward, L to cycle back)
    You can change the position of the selected light
          'x' and 'X' to move forward and back along the x-axis
          'y' and 'Y' to move forward and back along the y-axis
          'z' and 'Z' to move forward and back along the z-axis
'r' resets the scene
'q' to quit
