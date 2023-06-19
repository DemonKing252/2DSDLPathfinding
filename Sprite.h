#pragma once
#include <SDL.h>
enum texture_type
{
	def,
	grass,
	swamp,
	rock,
	stone
};

class Sprite
{
protected:
	SDL_Texture* m_pTexture;
	SDL_Rect m_rSrc;
	texture_type m_type;
public:
	void SetSprite(SDL_Texture* pTexture, char c);
	SDL_Texture* GetTexture() const;
	SDL_Rect* GetSrc();

};

