## modsBootstrap

modsBootstrap is a simple static library that provides the basic interface for creating an OpenGL application.  
This project is an assignment for implementing computer graphics in C++.

TODO List:
- Make window wrapper so more than one window can be active at a time
- Input class that is an extension (not derived) for the window wrapper so the window wrapper is cluttered
The input class should handle all input and allow bindings of actions and axes
- Base application class that handles all systems
- Base class for buffers, it should handle the binding and unbinding for the buffers

Wish List:
- Game object (Actor) feature
- A component feature
- Custom file format that resources such as textures and models are saved out to
This format will allow for a single path to be provided and will contain the additional
details related to that asset. Importing options will be available (jpg imports as texture)
- Update (research) build output and resources location so builds are nicely packed
