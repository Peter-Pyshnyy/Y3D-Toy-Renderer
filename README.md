A personal OpenGL renderer built for learning and experimenting with modern graphics techniques, rendering algorithms, and engine architecture concepts.

<img width="1272" height="712" alt="image" src="https://github.com/user-attachments/assets/e75e23b9-42ca-4c3a-9869-409fdcbc1870" />
<img width="804" height="545" alt="image" src="https://github.com/user-attachments/assets/3ed37177-94ab-447f-9af5-5b633174094c" />








**Building Notes**

- All dependencies are either included in the project or fetched using CMake
- On some machines running Visual Studio, the project may fail to configure or build properly unless the CMake generator is explicitly set to "Visual Studio 17 2022 Win64"
- Expect the first compilation to take longer

**Features**


- UML diagram: [`docs/UML.md`](docs/UML.md)
- Modular architecture
- Multi-shader pipeline
- Model loading
- Hierarchical scene graph with transform propagation
- Scene nodes include: multiple light types, 3d models, procedurally generated geometry
- GUI, including hierarchy and properties windows, as well as a gizmo for transformations

**Soon to be added**

- Scene saving/loading via JSON
- More complex lighting system
