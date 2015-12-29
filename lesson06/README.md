Lesson 6
=========
This lesson packs a lot, so bear with me.

Structurally, what changes here is that this program does two more things the last didn't do:
- Load a bitmap asset and offer it to native code, so it's pixels can be copied into the native heap.
- Load said bitmap data as a texture into a Sampler uniform, so it can be used for extracting fragment colour data.

Each of these steps require a few new additions.
- To copy the bitmap data in native code, we have to lock it's pixel data, obtain a pointer to it, allocate a buffer and copy the data into it. Then, we can finally release the original bitmap.
- Once we have the newly-created buffer, we can finally place into a texturing unit.
- We  must remember to place said texturing unit into the sampler
- Oh, and look out for texturing coordinates and shader compilation. Since we're not using the colours anymore, we MUST take everything related to it out, risking a GL error (since the shader position is optimized out and we don't to send useless data into the GPU).

You must be thinking "Were do we say what texturing unit are we using?!".
Well, since we never change it from default, we're implictly using GL_TEXTURE0.
