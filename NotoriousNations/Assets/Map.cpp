#include "Map.hpp"

Map::Map()
{
	int2_size = Int2(0, 0);
	m_i_tile_tiles = std::map<int, Tile>();
}

Map::Map(std::string s_name, std::map<int, Tile> m_i_tile_tiles, std::string s_shape, int width) : Asset(s_name)
{
	int2_size = Int2(width, static_cast<int>(m_i_tile_tiles.size() / width));

	this->s_shape = s_shape;

	this->m_i_tile_tiles = std::move(m_i_tile_tiles);
}

std::shared_ptr<sf::Texture> Map::p_txtr_get_soil_cover_texture() { return p_txtr_soil_cover_texture; }

void Map::update_soil_cover_texture(std::shared_ptr<sf::Texture> p_txtr_soil_cover_atlas, int i_tile_size)
{
	sf::RenderTexture rtxr_temp_texture;

	sf::Sprite sprt_temp_sprite = sf::Sprite(*p_txtr_soil_cover_atlas);

	Int2 int2_atlas_coords;

	rtxr_temp_texture.create(i_tile_size * int2_size.x, i_tile_size  * int2_size.y);

	for (int i = 0; i < int2_size.x * int2_size.y; i++)
	{
		std::wcout << m_i_tile_tiles[i].slcv_get_soil_cover().s_get_name().c_str() << L"\n";
		int2_atlas_coords = m_i_tile_tiles[i].slcv_get_soil_cover().int2_get_atlas_coords();
		std::wcout << std::to_wstring(int2_atlas_coords.x) << L" " << std::to_wstring(int2_atlas_coords.y) << L"\n";
		sprt_temp_sprite.setTextureRect(sf::Rect(int2_atlas_coords.x * i_tile_size,
			((int)p_txtr_soil_cover_atlas->getSize().y) - i_tile_size - int2_atlas_coords.y * i_tile_size, i_tile_size, i_tile_size));
		sprt_temp_sprite.setPosition(static_cast<float>((i % int2_size.x) * i_tile_size), static_cast<float>((i / int2_size.x) * i_tile_size));
		rtxr_temp_texture.draw(sprt_temp_sprite);
	}

	p_txtr_soil_cover_texture = std::make_shared<sf::Texture>(sf::Texture(rtxr_temp_texture.getTexture()));
}