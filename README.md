# Shackle
An old game engine stored here for historical and personal reasons. I have no plans to update this at the moment. 
As I remember the library works, but it has not seen actual usage other than testing so there are a lot of rough edges
and things that could have been implemented in a more robust way.

Requirements( versions circa summer 2012 )
- OGre3D
- CEGUI 
- OIS
- Boost threadpool
- cxxtest (for unittests)

Features: 
- Context based input handling
- Loading blender .scenefiles( copied from existing implementation ) 
- Main loop
- task multithreading
- lua bindings

Additional tools
- Unit tests( with cxxtest )
- doxygen integration
- Performance analysis and code coverage( with gcov and lcov )

The original idea was to create a library to be used as a base for my future game. 
I tried implementing a game at the same time using this, but the game ended up 
being only a test bed because of large amount of needed features. The library as of 
now i more like a technical demo of me using different tools and techniques rather than an
actual game engine. The name Shackle comes from a thought that because of my lack of 
experience at the time and lack of proper usage, whatever I'm creating here would 
probably be a limitation and need reimplementing at some point.
