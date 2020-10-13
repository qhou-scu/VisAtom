#include "Style.h"
GLint Style::curStyle=Style_default;

Style::Style()
{
	oldStyle = curStyle;
	SetStyle(curStyle);
}

Style::~Style()
{
  RestoreOldStyle();
}

void Style::SetStyle(GLint i)
{
	switch (i)
	{
	case Style_default: 
			 Solid();
			 break;
	case Style_SOLID :
			 Solid();
			 break;
	case Style_SOLID_FRONT_ONLY:
			 SolidFrontOnly();
			 break;
	case Style_SOLID_BACK_ONLY:
			 SolidBackOnly();
			 break;
	case Style_WIRE:
			 Wire();
			 break;
	case Style_TRANSP_SOLID:
			 TranspSolid();
			 break;
	 }
}


void Style::SolidFrontOnly()
{
	
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDisable(GL_BLEND);
	glPolygonMode(GL_FRONT,GL_FILL);
	//glBlendFunc(GL_SRC_ALPHA_SATURATE,GL_ONE_MINUS_SRC_ALPHA);
	curStyle = Style_SOLID_FRONT_ONLY;
    return;
}

void Style::SolidBackOnly()
{
	glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
	glDisable(GL_BLEND);
	glPolygonMode(GL_BACK,GL_FILL);
	//glBlendFunc(GL_SRC_ALPHA_SATURATE,GL_ONE_MINUS_SRC_ALPHA);
 	curStyle = Style_SOLID_BACK_ONLY;
   return;
}

void Style::Solid()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	//glBlendFunc(GL_SRC_ALPHA_SATURATE,GL_ONE_MINUS_SRC_ALPHA);
	curStyle = Style_SOLID;
    return;
}

void Style::Wire()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//glBlendFunc(GL_SRC_ALPHA_SATURATE,GL_ONE_MINUS_SRC_ALPHA);
	curStyle = Style_WIRE;
    return;
}

void Style::TranspSolid()
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	//glBlendFunc(GL_ONE, GL_SRC_COLOR);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	curStyle = Style_TRANSP_SOLID;
    return;
}

void Style::RestoreOldStyle()
{
	SetStyle(oldStyle);
    return;
}

