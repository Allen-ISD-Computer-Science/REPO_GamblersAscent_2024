#include "Image_Render.h"

//constructor
Image_Render::Image_Render(SDL_Handler* handler, int imageWidth, int imageHeight)
	:m_handler(*handler), m_texture(nullptr), m_imageWidth(imageWidth), m_imageHeight(imageHeight) {
}

//destructor
Image_Render::~Image_Render() {
}

void Image_Render::render(SDL_Texture* texture, SDL_Rect srcrect, int imageX, int imageY) {
	m_texture = texture;
	
	SDL_Rect destrect = {  imageX, //topleft image position x axis (centered)
						   imageY, //topleft image position y axis (centered)
						   m_imageWidth, //image width 
						   m_imageHeight }; //image height
	m_handler.DrawRectangle(srcrect, destrect, m_texture);

}
// Renders using SDL_RenderCopyEx
void Image_Render::render(SDL_Texture* texture, SDL_Rect srcrect, SDL_Rect destrect, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	m_texture = texture;
	SDL_RenderCopyEx(m_handler.renderer, m_texture, &srcrect, &destrect, angle, NULL, flip);
}