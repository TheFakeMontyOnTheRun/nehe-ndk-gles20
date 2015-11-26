Lesson 3
=========
Compared with the last one, this is a rather tame project.

We begin by atributing indices (and thus, removing extraneous repeated vertices) and then adding the colour information.
Then, we must add a new vertex attribute into the shaders, by plugging in the provided arrays and enabling it. Finally, we must change the drawing operation, so that it will use said indices for referencing the geometry data.

Since we want to slowly learn new stuff, there is a new element here as well: the quad is now drawn as a triangle strip - thus requiring only one extra index, beyond the original triangle.

Next up: indexed geometry and atributes.
