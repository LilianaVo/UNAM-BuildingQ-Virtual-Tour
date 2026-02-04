<div align="center">

# 🏛️ Virtual Tour: UNAM Engineering Faculty (Building Q)

### An interactive 3D simulation and walkthrough of the "Luis G. Valdés Vallejo" building (2nd Floor).

![OpenGL](https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?style=for-the-badge&logo=opengl)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![VS](https://img.shields.io/badge/Visual%20Studio-5C2D91.svg?style=for-the-badge&logo=visual-studio&logoColor=white)
![UNAM](https://img.shields.io/badge/UNAM-Engineering-gold?style=for-the-badge)

[View Demo] • [Report Bug] • [Request Feature]

</div>

---

## 🖥️ Project Overview

This project consists of a **Virtual Digital Twin** of the second floor of Building Q at the UNAM Faculty of Engineering. Developed from scratch using **OpenGL**, the application allows users to navigate through a detailed 3D environment, simulating a realistic walkthrough of key academic spaces.

The system demonstrates advanced computer graphics concepts, including hierarchical modeling, texture mapping, and real-time user interaction, serving as a potential prototype for university virtual tours.

**📍 Modeled Areas:**
* **Microsoft AI Lab:** High-detail interior modeling.
* **CISCO Networking Lab:** Equipment and layout simulation.
* **Computer Graphics Lab:** The very lab where this course is taught.
* **Hallways & Common Areas:** Accurate architectural representation.

---

## Academic Context

This project was developed as the Final Capstone for the **Computer Graphics & HCI** course at **UNAM**.

| **Course Information** | **Details** |
| :--- | :--- |
| **University** | Universidad Nacional Autónoma de México (UNAM) |
| **Faculty** | **Faculty of Wngineering** |
| **Course** | Computer Graphics and Human-Computer Interaction |
| **Professor** | **Ing. Luis Sergio Valencia Castro** |
| **Semester** | 2025-02 |
| **Groups** | Theory: **03** | Lab: **10** |

---

## ⚙️ Technical Features

The engine was built using **C++** and the **OpenGL** graphics library, implementing the following techniques:

* **Hierarchical Modeling:** Complex objects constructed using parent-child relationships for articulated movement.
* **Camera System:** First-person navigation (FPS style) with collision detection boundaries.
* **Advanced Texturing:** UV mapping for realistic surfaces (floors, walls, equipment).
* **Lighting & Shading:** Implementation of Phong lighting model for depth and realism.
* **Animation:** Scripted movements for doors and interactive elements.

---

## Development Team

This project was a collaborative effort by the following engineering students:

| Student Name | Student ID (No. Cuenta) | Role |
| :--- | :--- | :--- |
| **González Nava Alicia Aislinn** | 317251357 | 3D Modeling & Texturing |
| **Román Jiménez Adolfo** | 410098363 | Core Engine & Physics |
| **Lee Obando Ileana Verónica** | 318118408 | Lighting & Scene Assembly |

---

## 🚀 Installation & Usage

To run the virtual tour on your local machine, follow these steps:

### Prerequisites
* Windows OS (Recommended).
* Visual Studio C++ Redistributables.
* Graphics Card with OpenGL 3.3+ support.

### Setup
1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/adolfo-roman/ProyectoFinalCG.git](https://github.com/adolfo-roman/ProyectoFinalCG.git)
    ```
2.  **Extract Assets:**
    * Locate the `.rar` file inside the root folder (or Release section).
    * **Important:** Unzip the contents into the main project directory to ensure the executable finds the texture paths correctly.
3.  **Run:**
    * Launch the `.exe` file (e.g., `ProyectoFinal.exe`).

### Controls
* `W`, `A`, `S`, `D`: Move Camera
* `Mouse`: Look around
* `ESC`: Exit application

---

## 📄 License

*All 3D models and textures used in this project are either original or royalty-free.*
Developed for educational purposes at UNAM Engineering Faculty - 2025.
