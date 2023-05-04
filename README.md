# PBFluids - A Maya Plugin for PB Fluids

## Authors: Jiefu Ling, Hao Wu, advised by Dr. Stephen Lane

We hope to help **artists, designers, and students** create **fast and stable** fluid simulation that can **interact with Maya objects** they already have in scene.

## Included in This Repo

**PBFluidsGL**: A naive test of basic algorithm rendered with OpenGL. It uses the same data structure as our final product, but does not permit user control, and lack advanced features such as custom containers, mesh decomposition etc.

**PBFluids**: Complete maya plugin for decomposing custom 3D mesh into particles, and simulate its interaction with custom container, allowing full control of parameters.

## References
Mesh to Point Cloud: [WOF C++ Library](https://www.geom.at/wof-point-cloud-mesher-doc/html/index.html)
Adapted from *Position Based Fluids* by Miles Macklin, Matthias Müller from NVIDIA, presented in SIGGRAPH 2013

---

## How to Build & Run

### Required software
Autodesk Maya 2022(+)
Visual Studio 2019, or 2019+ w/ Visual Studio 2019 Build Tools

### Environment variables
MAYA_LOCATION: Configured when Maya is installed

PATH: %MAYA_LOCATION%\bin, YOUR_PROJECT_LOCATION\PBFluids\wofReleasePURE_v1.13\binaries\x64

## PBFluidsGL:
  Directly build and run project in Visual Studio
  
## PBFluids:
  1. Build project in Visual Studio.
  
![image](https://user-images.githubusercontent.com/25204397/235379915-c913553a-1da9-4b20-90ce-158ef5dc280a.png)

  2. When completed, start Maya. Plugin should be automatically loaded on Maya startup.
  
![image](https://user-images.githubusercontent.com/25204397/235379410-0aa4bede-d876-476b-afa0-312f79deef7a.png)

  3. Adjust particle and simulation parameters. Optionally, select custom meshes as initial fluid state and container.
  
![image](https://user-images.githubusercontent.com/25204397/235379695-e45ae9e2-90aa-4fcc-8cb2-445d2b4fa1f7.png)

  4. Click "Generate" and wait for animation to finish.

![image](https://user-images.githubusercontent.com/25204397/235379790-e50e9baa-344b-4e40-b0eb-82307c1564eb.png)

  5. Tune parameters until you are satisfied.
  
![image](https://user-images.githubusercontent.com/25204397/235379831-09431295-5559-4303-a03a-aeeeba8615b0.png)

  6. Click "Export" to export a video animation. The video will open in your default video editor.

![image](https://user-images.githubusercontent.com/25204397/235379863-9b8dc83f-1cdc-4f34-b1fe-52b63f9c9dad.png)

---

## Sample results
See images and videos included in "Results" folder. Two sample Maya scenes are also included in "PBFluids" folder for testing with PBFluids.

<img width="834" alt="PBFluids Node" src="https://user-images.githubusercontent.com/25204397/235381117-f4780fab-3431-44a5-878f-111c2a6cf8ae.png">
<img width="681" alt="Sample Frame" src="https://user-images.githubusercontent.com/25204397/235381128-fdb717d2-eb78-468c-80c0-5d647ef31788.png">
<img width="761" alt="Screenshot 2023-04-30 192553" src="https://user-images.githubusercontent.com/25204397/235381142-6caa3985-d4c9-4743-8b2c-672a8fc031c4.png">

## Video demo


https://user-images.githubusercontent.com/25204397/235381004-1ef9fe68-072b-48ca-8d28-3617fb8b8658.mp4


[Youtube](https://youtu.be/lNftHlAiMlM)
