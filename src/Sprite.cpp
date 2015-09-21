#include "GLincludes.h"

#include "Sprite.h"

#include "Texture.h"

#include "Utils.h"

#include "Color.h"

#include "Game.h"
extern Game *game;

Sprite::Sprite()
{
	mpTexture = NULL;
}

Sprite::Sprite(Texture *texture)
{
	mpTexture = texture;
	mScale = Vec2d(1,1);
	mSize = Vec2d(mpTexture->getWidth(), mpTexture->getHeight());
}

Sprite::Sprite(Texture *texture, Vec2d scale)
{
	mpTexture = texture;
	mScale = scale;
}

Sprite::Sprite(Texture *texture, Vec2d location, Vec2d size)
{
	mpTexture = texture;
	mTexPosition = location;
	mSize = size;
}

Sprite::Sprite(Texture *texture, Vec2d location, Vec2d size, Vec2d scale)
{
	mpTexture = texture;
	mTexPosition = location;
	mSize = size;
	mScale = scale;
}

//Peter: Used great billboard drawing trick from http://www.lighthouse3d.com/opengl/billboarding/index.php?billCheat.
void Sprite::draw(Vec3d &location)
{

	GLfloat left = convertScale(mTexPosition.X(), 0, mpTexture->getWidth(), 0.0f, 1.0f);
	GLfloat right = convertScale(mTexPosition.X() + mSize.X(), 0, mpTexture->getWidth(), 0.0f, 1.0f);

	GLfloat up = convertScale(mTexPosition.Y() + mSize.Y(), 0, mpTexture->getHeight(), 1.0f, 0.0f);
	GLfloat down = convertScale(mTexPosition.Y(), 0, mpTexture->getHeight(), 1.0f, 0.0f);

	glPushMatrix();

	glTranslatef(location.x, location.y, location.z );

	glPushMatrix();

	//glTranslatef(mpTexture->getRatio()*0.5,0,0);

	mpTexture->setAsActiveTexture();

	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	// beware all scaling is lost as well 
	for( i=0; i<3; i++ ) 
		for( j=0; j<3; j++ ) 
		{
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}

	// set the modelview with no rotations and scaling
	glLoadMatrixf(modelview);

	glBegin(GL_QUADS);
	// Front Face
	glColor3f(1,1,1);

	glTexCoord2f(left,up);
	glVertex3f(0.0f, -0.5f, 0.0f);              // Top Left

	glTexCoord2f(right,up);
	glVertex3f( /*mpTexture->getRatio()*/(mSize.X()/mSize.Y())*1.0f, -0.5f, 0.0f);              // Top Right

	glTexCoord2f(right,down);
	glVertex3f( /*mpTexture->getRatio()*/(mSize.X()/mSize.Y())*1.0f,0.5f, 0.0f);              // Bottom Right

	glTexCoord2f(left,down);
	glVertex3f(0.0f,0.5f, 0.0f);              // Bottom Left

	glEnd();

	glPopMatrix();

	// restores the modelview matrix
	glPopMatrix();

	glPopMatrix();
}

//Peter: Used great billboard drawing trick from http://www.lighthouse3d.com/opengl/billboarding/index.php?billCheat.
void Sprite::drawScaled(Vec3d &location, Vec2d &scale)
{

	GLfloat left = convertScale(mTexPosition.X(), 0, mpTexture->getWidth(), 0.0f, 1.0f);
	GLfloat right = convertScale(mTexPosition.X() + mSize.X(), 0, mpTexture->getWidth(), 0.0f, 1.0f);

	GLfloat up = convertScale(mTexPosition.Y() + mSize.Y(), 0, mpTexture->getHeight(), 1.0f, 0.0f);
	GLfloat down = convertScale(mTexPosition.Y(), 0, mpTexture->getHeight(), 1.0f, 0.0f);

	glPushMatrix();

	glTranslatef(location.x, location.y, location.z);

	glPushMatrix();

	//glTranslatef(mpTexture->getRatio()*0.5,0,0);

	mpTexture->setAsActiveTexture();

	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	// beware all scaling is lost as well 
	for( i=0; i<3; i++ ) 
		for( j=0; j<3; j++ ) 
		{
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}

	// set the modelview with no rotations and scaling
	glLoadMatrixf(modelview);

	glBegin(GL_QUADS);
	// Front Face
	glColor3f(1,1,1);

	glTexCoord2f(left,up);
	glVertex3f(-((mSize.X()/mSize.Y()) * scale.X())/2, -scale.Y()/2, 0.0f);              // Top Left

	glTexCoord2f(right,up);
	glVertex3f(/*mpTexture->getRatio()*/((mSize.X()/mSize.Y()) * scale.X())/2, -scale.Y()/2, 0.0f);              // Top Right

	glTexCoord2f(right,down);
	glVertex3f( /*mpTexture->getRatio()*/((mSize.X()/mSize.Y()) * scale.X())/2, scale.Y()/2, 0.0f);              // Bottom Right

	glTexCoord2f(left,down);
	glVertex3f(-((mSize.X()/mSize.Y()) * scale.X())/2,scale.Y()/2, 0.0f);              // Bottom Left

	glEnd();

	glPopMatrix();

	// restores the modelview matrix
	glPopMatrix();

	glPopMatrix();
}

//Peter: Used great billboard drawing trick from http://www.lighthouse3d.com/opengl/billboarding/index.php?billCheat.
void Sprite::drawScaledTinted(Vec3d &location, Vec2d &scale, Color &color)
{

	GLfloat left = convertScale(mTexPosition.X(), 0, mpTexture->getWidth(), 0.0f, 1.0f);
	GLfloat right = convertScale(mTexPosition.X() + mSize.X(), 0, mpTexture->getWidth(), 0.0f, 1.0f);

	GLfloat up = convertScale(mTexPosition.Y() + mSize.Y(), 0, mpTexture->getHeight(), 1.0f, 0.0f);
	GLfloat down = convertScale(mTexPosition.Y(), 0, mpTexture->getHeight(), 1.0f, 0.0f);

	glPushMatrix();

	glTranslatef(location.x, location.y, location.z);

	glPushMatrix();

	//glTranslatef(mpTexture->getRatio()*0.5,0,0);

	mpTexture->setAsActiveTexture();

	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	// beware all scaling is lost as well 
	for( i=0; i<3; i++ ) 
		for( j=0; j<3; j++ ) 
		{
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}

	// set the modelview with no rotations and scaling
	glLoadMatrixf(modelview);

	glBegin(GL_QUADS);
	// Front Face

	//Set tint.
	glColor3f(color.r, color.g, color.b);

	glTexCoord2f(left,up);
	glVertex3f(-((mSize.X()/mSize.Y()) * scale.X())/2, -scale.Y()/2, 0.0f);              // Top Left

	glTexCoord2f(right,up);
	glVertex3f(/*mpTexture->getRatio()*/((mSize.X()/mSize.Y()) * scale.X())/2, -scale.Y()/2, 0.0f);              // Top Right

	glTexCoord2f(right,down);
	glVertex3f( /*mpTexture->getRatio()*/((mSize.X()/mSize.Y()) * scale.X())/2, scale.Y()/2, 0.0f);              // Bottom Right

	glTexCoord2f(left,down);
	glVertex3f(-((mSize.X()/mSize.Y()) * scale.X())/2,scale.Y()/2, 0.0f);              // Bottom Left

	glEnd();

	glPopMatrix();

	// restores the modelview matrix
	glPopMatrix();

	glPopMatrix();
}

//Peter: Used great billboard drawing trick from http://www.lighthouse3d.com/opengl/billboarding/index.php?billCheat.
void Sprite::drawScaledTintedRotated(Vec3d &location, Vec2d &scale, Color &color, float rotation)
{

	GLfloat left = convertScale(mTexPosition.X(), 0, mpTexture->getWidth(), 0.0f, 1.0f);
	GLfloat right = convertScale(mTexPosition.X() + mSize.X(), 0, mpTexture->getWidth(), 0.0f, 1.0f);

	GLfloat up = convertScale(mTexPosition.Y() + mSize.Y(), 0, mpTexture->getHeight(), 1.0f, 0.0f);
	GLfloat down = convertScale(mTexPosition.Y(), 0, mpTexture->getHeight(), 1.0f, 0.0f);

	glPushMatrix();

	glTranslatef(location.x, location.y, location.z);

	glPushMatrix();

	//glTranslatef(mpTexture->getRatio()*0.5,0,0);

	mpTexture->setAsActiveTexture();

	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	// beware all scaling is lost as well 
	for( i=0; i<3; i++ ) 
		for( j=0; j<3; j++ ) 
		{
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}

	// set the modelview with no rotations and scaling
	glLoadMatrixf(modelview);

	glRotatef(rotation, 0.0, 0.0, 1.0);

	//cwc::glShader *shader = (*game->getShaderManager())[0];

	//shader->begin();

	glBegin(GL_QUADS);
	// Front Face

	//Set tint.
	glColor3f(color.r, color.g, color.b);

	glTexCoord2f(left,up);
	glVertex3f(-((mSize.X()/mSize.Y()) * scale.X())/2, -scale.Y()/2, 0.0f);              // Top Left

	glTexCoord2f(right,up);
	glVertex3f(/*mpTexture->getRatio()*/((mSize.X()/mSize.Y()) * scale.X())/2, -scale.Y()/2, 0.0f);              // Top Right

	glTexCoord2f(right,down);
	glVertex3f( /*mpTexture->getRatio()*/((mSize.X()/mSize.Y()) * scale.X())/2, scale.Y()/2, 0.0f);              // Bottom Right

	glTexCoord2f(left,down);
	glVertex3f(-((mSize.X()/mSize.Y()) * scale.X())/2,scale.Y()/2, 0.0f);              // Bottom Left

	glEnd();

	//shader->end();

	if(game->getWireframeBoundingBoxMode())
	{
		glDisable(GL_TEXTURE_2D);

		glLineWidth(10);

		glBegin(GL_LINES);
		// Front Face

		//Set tint.
		glColor3f(color.r, color.g, color.b);

		glTexCoord2f(left,up);
		glVertex3f(-((mSize.X()/mSize.Y()) * scale.X())/2, -scale.Y()/2, 0.0f);              // Top Left
		glVertex3f(/*mpTexture->getRatio()*/((mSize.X()/mSize.Y()) * scale.X())/2, -scale.Y()/2, 0.0f);

		glTexCoord2f(right,up);
		glVertex3f(/*mpTexture->getRatio()*/((mSize.X()/mSize.Y()) * scale.X())/2, -scale.Y()/2, 0.0f);              // Top Right
		glVertex3f( /*mpTexture->getRatio()*/((mSize.X()/mSize.Y()) * scale.X())/2, scale.Y()/2, 0.0f);

		glTexCoord2f(right,down);
		glVertex3f( /*mpTexture->getRatio()*/((mSize.X()/mSize.Y()) * scale.X())/2, scale.Y()/2, 0.0f);              // Bottom Right
		glVertex3f(-((mSize.X()/mSize.Y()) * scale.X())/2,scale.Y()/2, 0.0f);

		glTexCoord2f(left,down);
		glVertex3f(-((mSize.X()/mSize.Y()) * scale.X())/2,scale.Y()/2, 0.0f);              // Bottom Left
		glVertex3f(-((mSize.X()/mSize.Y()) * scale.X())/2, -scale.Y()/2, 0.0f);

		glEnd();

		glEnable(GL_TEXTURE_2D);
	}

	glPopMatrix();

	// restores the modelview matrix
	glPopMatrix();

	glPopMatrix();
}

void Sprite::drawPlain(Vec2d &location, Vec2d &bounds, Vec2d &scale)
{
	GLfloat left = convertScale(mTexPosition.X(), 0, mpTexture->getWidth(), 0.0f, 1.0f);
	GLfloat right = convertScale(mTexPosition.X() + bounds.X(), 0, mpTexture->getWidth(), 0.0f, 1.0f);

	GLfloat up = convertScale(mTexPosition.Y() + bounds.Y(), 0, mpTexture->getHeight(), 1.0f, 0.0f);
	GLfloat down = convertScale(mTexPosition.Y(), 0, mpTexture->getHeight(), 1.0f, 0.0f);

	// Front Face

	glTexCoord2f(left,up);
	glVertex3f(-((bounds.X()/bounds.Y()) * scale.X())/2, -scale.Y()/2, 0.0f);              // Top Left

	glTexCoord2f(right,up);
	glVertex3f(/*mpTexture->getRatio()*/((bounds.X()/bounds.Y()) * scale.X())/2, -scale.Y()/2, 0.0f);              // Top Right

	glTexCoord2f(right,down);
	glVertex3f( /*mpTexture->getRatio()*/((bounds.X()/bounds.Y()) * scale.X())/2, scale.Y()/2, 0.0f);              // Bottom Right

	glTexCoord2f(left,down);
	glVertex3f(-((bounds.X()/bounds.Y()) * scale.X())/2,scale.Y()/2, 0.0f);              // Bottom Left

}
