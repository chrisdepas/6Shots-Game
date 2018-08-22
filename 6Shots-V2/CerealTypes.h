#ifndef __CEREAL_TYPES_H__
#define __CEREAL_TYPES_H__

/*
**	Cereal Serialization functions for SFML Types
**	C De Pasquale 2/06/18
**
**
**
*/

// DEFAULT TO USING NAME-VALUE PAIRS
#ifndef CEREAL_SFML_NVP
	#define CEREAL_SFML_NVP 1
#endif

#if CEREAL_SFML_NVP == 1
	namespace sf {
		// Vectors
		template<class Archive> void serialize(Archive& ar, sf::Vector2i &v) { ar(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y)); }
		template<class Archive> void serialize(Archive& ar, sf::Vector2f &v) { ar(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y)); }
		template<class Archive> void serialize(Archive& ar, sf::Vector2u &v) { ar(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y)); }
		template<class Archive> void serialize(Archive& ar, sf::Vector3i &v) { ar(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y), cereal::make_nvp("z", v.z)); }
		template<class Archive> void serialize(Archive& ar, sf::Vector3f &v) { ar(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y), cereal::make_nvp("z", v.z)); }

		// Color
		template<class Archive> void serialize(Archive& ar, sf::Color &c) { ar(cereal::make_nvp("r", c.r), cereal::make_nvp("g", c.g), cereal::make_nvp("b", c.b), cereal::make_nvp("a", c.a)); }
	
		// IpAddress
		template<class Archive> void serialize(Archive& ar, sf::IpAddress &ip) { ar(cereal::make_nvp("r", ip.toString())); }

		// Time
		template<class Archive> void serialize(Archive& ar, sf::Time &t) { ar(cereal::make_nvp("time_us", t.asMicroseconds())); }

		// Image - Save
		template<class Archive> void save(Archive& ar, const sf::Image &img) {
			sf::Uint32* pImg = (sf::Uint32*)img.getPixelsPtr();

			// Write size
			sf::Vector2u size = img.getSize();
			ar(cereal::make_nvp("img_size", size));

			// Write each pixel RGBA as uint32
			for (unsigned int i = 0; i < (size.x*size.y); i++) {
				ar(
					cereal::make_nvp(std::to_string(i),
					*(const sf::Uint32*)(pImg + i))
				);
			}
		}
		// Image - Load
		template<class Archive> void load(Archive& ar, sf::Image &img) {
			// Read image size
			sf::Vector2u size;
			ar(cereal::make_nvp("img_size", size));

			// Attempt to create texture with given size. If texture size fails,
			// It is probably because the size is above the max texture size 
			// supported by the graphics card
			if (!img.create(size.x, size.y)) {
				throw std::runtime_error(std::string("Invalid image size ") \
					+ std::to_string(size.x) + ", " + std::to_string(size.y));
			}

			// Read each pixel data into buffer
			auto buffer = std::make_unique<sf::Uint32[]>(size.x*size.y);
			for (unsigned int i = 0; i < (size.x*size.y); i++) {
				ar(
					cereal::make_nvp(std::to_string(i), 
					buffer[i])
				);
			}

			// update image with pixel buffer
			img.update((sf::Uint8*)buffer.get());
		}

		// Texture  - Save
		template<class Archive> void save(Archive& ar, const sf::Texture &t) {
			// Copy texture from graphics card to main memory
			sf::Image img = t.copyToImage();
			sf::Uint32* pImg = (sf::Uint32*)img.getPixelsPtr();

			// Write size
			sf::Vector2u size = img.getSize();
			ar(cereal::make_nvp("size", size));

			// Write each pixel RGBA as uint32
			for (unsigned int i = 0; i < (size.x*size.y); i++) {
				ar(
					cereal::make_nvp(std::to_string(i),  
					*(const sf::Uint32*)(pImg+i))
				);
			}
		}
		// Texture - load
		template<class Archive> void load(Archive& ar, sf::Texture &t) {
			// Read texture size
			sf::Vector2u size;
			ar(cereal::make_nvp("size", size));

			// Attempt to create texture with given size. If texture size fails,
			// It is probably because the size is above the max texture size 
			// supported by the graphics card
			if (!t.create(size.x, size.y)) {
				throw std::runtime_error(std::string("Invalid texture size ") \
					+ std::to_string(size.x) + ", " + std::to_string(size.y));
			}

			// Read each pixel data into buffer
			auto buffer = std::make_unique<sf::Uint32[]>(size.x*size.y);
			for (unsigned int i = 0; i < size.x*size.y; i++) {
				ar(cereal::make_nvp(std::to_string(i), buffer[i]));
			}

			// update texture with pixel buffer
			t.update((sf::Uint8*)buffer.get());
		}

		// React
		template<class Archive> void serialize(Archive& ar, sf::Rect<float> &rf) { 
			ar(cereal::make_nvp("left_f", rf.left)); ar(cereal::make_nvp("top_f", rf.top)); ar(cereal::make_nvp("width_f", rf.width)); ar(cereal::make_nvp("height_f", rf.height));
		}
		template<class Archive> void serialize(Archive& ar, sf::Rect<int> &ri) {
			ar(cereal::make_nvp("left_i", ri.left)); ar(cereal::make_nvp("top_i", ri.top)); ar(cereal::make_nvp("width_i", ri.width)); ar(cereal::make_nvp("height_i", ri.height));
		}
		template<class Archive> void serialize(Archive& ar, sf::Rect<unsigned int> &ri) {
			ar(cereal::make_nvp("left_ui", ri.left)); ar(cereal::make_nvp("top_ui", ri.top)); ar(cereal::make_nvp("width_ui", ri.width)); ar(cereal::make_nvp("height_ui", ri.height));
		}
	};
#else 
	namespace sf {
		// Vectors
		template<class Archive> void serialize(Archive& ar, sf::Vector2i &v) { ar(v.x, v.y); }
		template<class Archive> void serialize(Archive& ar, sf::Vector2f &v) { ar(v.x, v.y); }
		template<class Archive> void serialize(Archive& ar, sf::Vector2u &v) { ar(v.x, v.y); }
		template<class Archive> void serialize(Archive& ar, sf::Vector3i &v) { ar(v.x, v.y, v.z); }
		template<class Archive> void serialize(Archive& ar, sf::Vector3f &v) { ar(v.x, v.y, v.z); }

		// Color
		template<class Archive> void serialize(Archive& ar, sf::Color &c) { ar(c.r, c.g, c.b, c.a); }

		// IpAddress
		template<class Archive> void serialize(Archive& ar, sf::IpAddress &ip) { ar(ip.toString()); }

		// Texture 
		template<class Archive> void save(Archive& ar, const sf::Texture &t) {
			// Copy texture from graphics card to main memory
			sf::Image img = t.copyToImage();
			sf::Uint32* pImg = (sf::Uint32*)img.getPixelsPtr();

			// Write size
			sf::Vector2u size = img.getSize();
			ar(size);

			// Write each pixel RGBA as uint32
			for (unsigned int i = 0; i < (size.x*size.y); i++) {
				ar(*(const sf::Uint32*)(pImg+i));
			}
		}

		template<class Archive> void load(Archive& ar, sf::Texture &t) {
			// Read texture size
			sf::Vector2u size;
			ar(size);

			// Attempt to create texture with given size. If texture size fails,
			// It is probably because the size is above the max texture size 
			// supported by the graphics card
			if (!t.create(size.x, size.y)) {
				throw std::runtime_error(std::string("Invalid texture size ") \
					+ std::to_string(size.x) + ", " + std::to_string(size.y));
			}

			// Read each pixel data into buffer
			auto buffer = std::make_unique<sf::Uint32[]>(size.x*size.y);
			for (unsigned int i = 0; i < size.x*size.y; i++) {
				ar(buffer[i]);
			}

			// update texture with pixel buffer
			t.update((sf::Uint8*)buffer.get());
		}
	};

#endif

#endif