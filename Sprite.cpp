#include "Sprite.h"
#include "Node.h"
void Sprite::SetSprite(SDL_Texture* pTexture, char c)
{
	this->m_pTexture = pTexture;
	switch (c) {
	case '.':
		static_cast<Node*>(this)->SetNavitation(navigation_type::open);
		m_type = texture_type::grass;
		m_rSrc = { 0, 0, 32, 32 };
		break;
	case 'x':
		static_cast<Node*>(this)->SetNavitation(navigation_type::closed);
		m_type = texture_type::rock;
		m_rSrc = { 64, 0, 32, 32 };
		break;
	case ';':
		static_cast<Node*>(this)->SetNavitation(navigation_type::open);
		m_type = texture_type::swamp;
		m_rSrc = { 32, 0, 32, 32 };
		break;
	case '-':
		static_cast<Node*>(this)->SetNavitation(navigation_type::closed);
		m_type = texture_type::stone;
		m_rSrc = { 160, 32, 32, 32 };
		break;
	default:
		static_cast<Node*>(this)->SetNavitation(navigation_type::open);
		m_type = texture_type::grass;
		m_rSrc = { 0, 0, 32, 32 };
		break;
	}
}

SDL_Texture* Sprite::GetTexture() const
{
	return m_pTexture;
}

SDL_Rect* Sprite::GetSrc()
{
	return &m_rSrc;
}
