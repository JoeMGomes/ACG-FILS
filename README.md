# Advanced Computer Graphics - Big Homework 2
 This project was developed durind the Advanced Computer Graphics course at Universitatea Politehnică din București.

 The aim of the project was to develop upon the given "game engine" base project adding more functionality and OpenGL features such as collisions, animation, terrain, skyboxes and dynamic water waves.
  ![Screenshots](/readmepics/batch.jpg)

# How to use
<kbd>Q</kbd> - Exit the program  

## Movement
<kbd>W</kbd> - Move forward  
<kbd>A</kbd> - Move left  
<kbd>S</kbd> - Move backward  
<kbd>D</kbd> - Move right  

There are to movement modes:  
 - *FPS Mode* <kbd>V</kbd> - While in *FPS Mode* the movement is constrained to the horizontal plane and gravity is applied.  
 - *Fly Mode* <kbd>C</kbd> - While in *Fly Mode* the player has no gravity and no collision with the floor (and only with the floor, objects and invisible walls still have collision). While in *Fly Mode* the <kbd>R</kbd> and <kbd>F</kbd> can be used to move directly up and down respectively. All the movement is applied in the camera looking direction.

## Camera
 The camera is controller with the mouse movement. The cursor can be released from the windows by pressing the <kbd>X</kbd> key. This allows the controll of the UI but does not move the camera anymore. Pressing <kbd>Z</kbd> will lock the cursor back in the scene and allow camera movement again.

 ## Rendering modes
 <kbd>B</kbd> - GL_FILL  
 <kbd>N</kbd> - GL_LINE

 # UI - Dear ImGui
 The project UI was implemented using [Dear ImGUI](https://github.com/ocornut/imgui).
 The GUI consists of a single window with 4 collapsing headers and a few straightforward option checkboxes. All the options allow the control of the scene in real time. The UI was also used as a development tool to help with debuging and positioning.

 ![Main GUI](/readmepics/GUI.jpg)
### Statistics
![Stats GUI](/readmepics/stats.jpg)
For this panel the **Counter** class was created. It consists of a static integer that is incremented everytime a draw function is called. Currently the project issues one draw call per object which is not the most efficient design but it is the simplest way of achieving what was needed.
### Light
![Light GUI](/readmepics/light.jpg)
### Camera
![Camera GUI](/readmepics/camera.jpg)
### Waves
![Waves GUI](/readmepics/waves.jpg)

# Implementation
## Collisions
 The collision in this project is performed via Axis Aligned Bounding Boxes (AABB). The bounding boxes sizes and position can be automatically calculated via a mesh vertex array or can be manually input. 

 There was an attempt to add the possibility to draw the bounding boxes but for unknown reasons to us it failed. The drawing code is still in the source files but is not used.

 Every frame, and since we only care about collision with the player in this project, the player's BB (Bounding Box) is checked for intersections against all other objects BB's that are considered relevant for collision. If a collision is detected that frame's movement is simply reverted.

 No complex collison detection methods like BSP or Octrees were implemented.

 To aid in the matching of the bounding box to the actual position of the meshes a very simple **GameObject** that groups a mesh, a bounding box and position, rotation and scale. This implementation is very crude and can (should!) be perfected.

 *Note*: The floor is a different case of the collision detection so it is done seperately from other collisions but still uses AABB. 
## Movement
To enable the *FPS Mode* the **Camera** class was expanded to allow FPS-like movement. This movement ignores the camera's pitch and moves the player only in the horizontal plane.
## Terrain Chunk

### **Height**

The mountain is a terrain chunk that contains an **n** x **n** mesh. The height of each vertex in the Mesh is calculated with the followig formula:

```
y = vHeight * eastRatio * maxHeight
```

* **vHeight** is a noise value generated with a fractal brownian noise algorithm provided by the library [**SimplexNoise**](https://github.com/SRombauts/SimplexNoise). This noise value interval is **[-1,1]** but **vHeight** is normalized to **[0,1]**.
* **eastRatio** is a ratio resulting from a smoothstep. The purpose of this ratio is to create a smooth trasition in the terrain from the mountains to the beach;
* **maxHeight** is the maximum value that a vertex can have.

### **Normals**

To calculate the normals of the mountain we did the following:  

* For each square of the mesh calculate the average normal:

```
  A -- D   
  |   /|
  |  / | 
  | /  |
  |/   |
  B -- C
```

Each vertex normal add the cross product of: **(AB, BC)**,**(BC, CD)**, **(CD, DA)** and **(DA, AB)** 

When you finish iterating through all the faces, normalize each normal.

### **Shader**

The mountain shader colors the mountain in various steps.

The default color (**resultTex**) is calculated using noise.

Then we have three different colorings each one with its own limit: **snow**, **grass**, **sand**, **water**. For example, only the vertices that are above a certain **y** and have the normal with a certain inclination are painted with white. 



## **OceanTile**

### **Mesh**

The algorithm for generating the mesh is similar to the terrainChunk but with every **y** is a 0.

### **Gerstner Waves**

For simulating the waves we use the a vertex shader given the complexity of the algorithm. We tried to implement the **Gerstner **Waves**** with [this](https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models) suggested approach.

However the normal calculation in the shader may have some errors.


## Animations
There are three different animations in the scene the loop indefinetely.  
 -    Spining Crab - walks in a circle passing bellow one chair
 -    Sinewave Crab - Walks in a sinusoidal motion switching direction with a step function.
 - Bouncing ball - Bounces according to a parabola and is compressed and stretched verticaly when hitting the floor.
  
## Skybox
The functionality was implemented in the **Skybox** class. The skybox consists of a cube that is drawn behind everything else in the scene. Changes to the "texture.cpp" file were needed in order to load *GL_TEXTURE_CUBE_MAP* properly.  

*Note:* The skybox implementation was adapted from Victor Gordan's youtube video linked [here](https://www.youtube.com/watch?v=8sVvxeKI9Pk&ab_channel=VictorGordan).  
*Note2:* There is currently a bug in the skybox where the texture does not blend seamlessly in some edges of the cube.
![Skybox GUI](/readmepics/skybox.jpg)

# Assets
All objects were free downloads from [Turbosquid.com](https://turbosquid.com)
- Crab - https://www.turbosquid.com/3d-models/cartoon-crab-bones-3ds-free/762723
- BeachBall - https://www.turbosquid.com/3d-models/swimming-pool-ball-3d-model-1572435
- Chair - https://www.turbosquid.com/3d-models/3d-chair-model-1172145
- Chest - https://www.turbosquid.com/3d-models/clash-chest-max-free/1039446 (was modified in [Blender](https://blender.org) to reduce polycount)




