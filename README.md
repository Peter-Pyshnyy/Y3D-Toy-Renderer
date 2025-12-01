A personal OpenGL renderer built for learning and experimenting with modern graphics techniques, rendering algorithms, and engine architecture concepts.

<img width="1272" height="712" alt="image" src="https://github.com/user-attachments/assets/e75e23b9-42ca-4c3a-9869-409fdcbc1870" />





**Building Notes**

- All dependencies are either included in the project or fetched using CMake
- On some machines running Visual Studio, the project may fail to configure or build properly unless the CMake generator is explicitly set to "Visual Studio 17 2022 Win64"
- Expect the first compilation to take longer

**Features**

- Modular architecture
- Multi-shader pipeline
- Model loading
- Hierarchical scene graph with transform propagation
- Scene nodes include: multiple light types, 3d models, procedurally generated geometry

**Soon to be added**

- Scene saving/loading via JSON
- GUI
- More complex lighting system
