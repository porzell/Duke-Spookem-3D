#include "Texture.h"

#ifdef WINDOWS
//Needed for PNG loading functions.
#include <libpng/png.h>

//windows.h needed for the APIENTRY dewfine used in GLU.h.
#include <Windows.h>

//Needed for gluErrorString().
#include <gl/GLU.h>

#pragma comment(lib, "libpng16.lib") // link statically with libpng16.lib
#else
#include <GL/glu.h>
#include <png.h>
#endif

Texture::Texture()
{
	//Set texture ID to NULL.
	mTextureID = NULL;

	//mSize will automatically set itself to (0,0).
	mShouldWrap = 0;
}

Texture::~Texture()
{
	//Free the textureID.
	freeTexture();
}

Texture::Texture( GLuint* pixels, GLuint width, GLuint height )
{
	loadTextureFromPixels32(pixels, width, height);
}

Texture::Texture(const char *path)
{
	if(!loadTextureFromPNG(path))
	{
		loadTextureFromBMP(path);
	}
}

Texture::Texture(std::string path)
{
	if(!loadTextureFromPNG(path.c_str()))
	{
		loadTextureFromBMP(path.c_str());
	}
}

//Deletes the current texture and nulls the texture GLuint.
void Texture::freeTexture()
{
	if(mTextureID)
	{
		glDeleteTextures( 1, &mTextureID );
		mTextureID = NULL;
	}
}

bool Texture::loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height )
{
	//Check if there is already a texture, and if so, free it.
	freeTexture();

	//Preemptively set width and height.
	mSize.set(width, height);

	//Generate texture ID.
	glGenTextures( 1, &mTextureID );

	//Bind texture ID.
	glBindTexture( GL_TEXTURE_2D, mTextureID );

	//Generate texture from array given, assuming 32-bit (RGBA).
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

	setFilters();
	facilitateWrap();

	//Unbind texture.
	glBindTexture( GL_TEXTURE_2D, NULL );

	//Check for error.
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
		return false;
	}
	return true;
}

//Note:
//Code adapted from NeheTutorials.
bool Texture::loadTextureFromBMP( const char *path )                   // Creates Texture From A Bitmap File
{
	//Check if there is already a texture, and if so, free it.
	freeTexture();

    HBITMAP hBMP;                                                       // Handle Of The Bitmap
    BITMAP  BMP;                                                        // Bitmap Structure
 
    glGenTextures(1, &mTextureID);                                           // Create The Texture
    hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL), path, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );
 
    if (!hBMP)                                                          // Does The Bitmap Exist?
        return false;                                                   // If Not Return False
 
    GetObject(hBMP, sizeof(BMP), &BMP);                                 // Get The Object
                                                                        // hBMP:        Handle To Graphics Object
                                                                        // sizeof(BMP): Size Of Buffer For Object Information
                                                                        // &BMP:        Buffer For Object Information
 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);                              // Pixel Storage Mode (Word Alignment / 4 Bytes)
 
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, mTextureID);                                // Bind To The Texture ID

    setFilters();
	facilitateWrap();

    glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
 
    DeleteObject(hBMP);                                                 // Delete The Object
 
    return true;                                                        // Loading Was Successful
}


//Note:
//Code adapted from http://stackoverflow.com/revisions/11297197/3, which in turn was revised from http://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures#A_simple_libpng_example.
//Had to also fix it when it was trying to read RGB instead of RGBA.
bool Texture::loadTextureFromPNG(const char *path)
{
	//Clear the errors
	glGetError();

	//Check if there is already a texture, and if so, free it.
	freeTexture();

	png_byte header[8];

	FILE *fp;
	
	fopen_s(&fp, path, "rb");
	if (fp == 0)
	{
		perror(path);
		return 0;
	}

	// read the header
	fread(header, 1, 8, fp);

	if (png_sig_cmp(header, 0, 8))
	{
		fprintf(stderr, "error: %s is not a PNG.\n", path);
		fclose(fp);
		return 0;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fprintf(stderr, "error: png_create_read_struct returned 0.\n");
		fclose(fp);
		return 0;
	}

	// create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}

	// create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return 0;
	}

	// the code in this if statement gets called if libpng encounters an error
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "error from libpng\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}

	// init png reading
	png_init_io(png_ptr, fp);

	// let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	// variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 temp_width, temp_height;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
		NULL, NULL, NULL);

	mSize.setWidth(temp_width);
	mSize.setHeight(temp_height);

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// glTexImage2d requires rows to be 4-byte aligned
	rowbytes += 3 - ((rowbytes-1) % 4);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte * image_data;
	image_data = (png_byte*)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
	if (image_data == NULL)
	{
		fprintf(stderr, "error: could not allocate memory for PNG image data\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}

	// row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep * row_pointers = (png_bytep*)malloc(temp_height * sizeof(png_bytep));
	if (row_pointers == NULL)
	{
		fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		free(image_data);
		fclose(fp);
		return 0;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	unsigned i;
	for (i = 0; i < temp_height; i++)
	{
		row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
	}

	// read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	// Generate the OpenGL texture object
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	GLuint out = GL_RGBA;

	if(color_type == PNG_COLOR_TYPE_RGB)
		out = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, out, temp_width, temp_height, 0, out, GL_UNSIGNED_BYTE, image_data);
	
	//Peter: This was in the original code, but I figured, hey, why not use a GL_LINEAR filter for some pizzazz?
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	setFilters();
	facilitateWrap();
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// clean up
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	free(image_data);
	free(row_pointers);
	fclose(fp);
	
	mTextureID = texture;

	//Unbind texture.
	glBindTexture( GL_TEXTURE_2D, NULL );

	//Check for error.
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error loading texture from filename: %s, %s\n", path, gluErrorString( error ) );
		return false;
	}

	return true;
}

void Texture::loadCheckerboard()
{
	freeTexture();

	glGenTextures(1, &mTextureID);

	glBindTexture(GL_TEXTURE_2D, mTextureID);

	float pixels[] = {
    0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
	};

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glGenerateMipmap(GL_TEXTURE_2D);

	setFilters();
	facilitateWrap();
}

void Texture::render(GLfloat x, GLfloat y)
{
	//If the texture exists
	if( mTextureID != 0 )
	{
		//Remove any previous transformations
		glLoadIdentity();

		//Move to rendering point
		glTranslatef( x, y, 0.f );

		//Set texture ID
		glBindTexture( GL_TEXTURE_2D, mTextureID );

		//Render textured quad
		glBegin( GL_QUADS );
			glTexCoord2f( 0.f, 0.f ); glVertex2f(		   0.f,			0.f );
			glTexCoord2f( 1.f, 0.f ); glVertex2f( (GLfloat)mSize.getWidth(),			0.f );
			glTexCoord2f( 1.f, 1.f ); glVertex2f( (GLfloat)mSize.getWidth(), (GLfloat)mSize.getHeight() );
			glTexCoord2f( 0.f, 1.f ); glVertex2f(		   0.f, (GLfloat)mSize.getHeight() );
		glEnd();
	}
}
