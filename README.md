3D Graphics Project
===================
*By: Dennis PLOTNIK, Gabriel BENEVIDES, Jakub Medvecký-Heretik*

**Controls**
Use the arrow keys (UP, DOWN, LEFT, RIGHT) to move the bear.

**Concept**
For our 3D scene we wanted to create something strange and psychedelic. Something that looked to be out of a dream. In order to accomplish this we allowed OpenGL to create a default material for the bear mesh that the player controls so that it looks as strange as possible with the multiple colours while still obviously being a bear. We also set the falling snowflakes to bounce back up into the air upon hitting the ground rather than lying still. This looked a little too out of place at first so we added a "snow repeller" in the center of the scene so as to explain the bouncing snowflakes.

**Technical Information**
In order to fulfill the project requirement of implementing something not covered in the practical sessions, we decided to procedurally generate the non-flat terrain. Using a 3-dimensional curve function and `GL_TRIANGLE_STRIP` we created a `Renderable` that has a `detail` property which would define how many vertices wide and long the terrain is as well as a `scale` property to increase the rendered size of the terrain. Using these properties, the `Renderable` creates a non-flat surface and colours it using another function based on altitude.

The mesh requirement is fulfilled by the bear we added to the scene. This bear is also controllable by the player using the keyboard. The other character in the scene is a snowman which was created using hierarchical rendering. The snowman has a key-frame animation to swing his hockey stick and hit the puck at the start of the scene. The snowman hits the puck into a hockey net that is constructed using a spring system. The scene takes place on a plane textured to look like a frozen lake with 3 multi coloured spot lights to add to the psychedelic atmosphere in the scene. The lake has an infinite collision plane on it so that the snowflakes can bounce and the puck can slide across the lake surface. The entire scene is also illuminated by a directional light meant to mimic a sun and is surrounded by a mountain textured skybox.
