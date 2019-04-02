/// @brief Include to sample cube map array textures.
/// @file gli/sampler_cube_array.hpp

#pragma once

#include "sampler.hpp"
#include "texture_cube_array.hpp"
#include "mipmaps_compute.hpp"
#include "convert_func.hpp"

#include "clear.hpp"

namespace gli
{
	/// Cube map array texture sampler
	/// @tparam T Sampler can fetch, write and interpret any texture format but will expose and process the data through type T conversions.
	/// @tparam P Precision in term of ULPs
	template <typename T, qualifier P = defaultp>
	class sampler_cube_array : public sampler
	{
	private:
		typedef typename detail::interpolate<T>::type interpolate_type;

	public:
		typedef texture_cube_array texture_type;
		typedef typename texture_type::size_type size_type;
		typedef typename texture_type::extent_type extent_type;
		typedef interpolate_type level_type;
		typedef vec<2, interpolate_type, P> normalized_type;
		typedef vec<4, T, P> texel_type;

		sampler_cube_array(texture_type const& Texture, wrap Wrap, filter Mip = FILTER_NEAREST, filter Min = FILTER_NEAREST, texel_type const& BorderColor = texel_type(0, 0, 0, 1));

        void set_border_color(texel_type BorderColor) {
            this->BorderColor = BorderColor;
        }

		/// Access the sampler texture object
		texture_type const& operator()() const;

		/// Fetch a texel from the sampler texture
		texel_type texel_fetch(extent_type const& TexelCoord, size_type layer, size_type Face, size_type Level) const;

		/// Write a texel in the sampler texture
		void texel_write(extent_type const& TexelCoord, size_type layer, size_type Face, size_type Level, texel_type const& Texel);

		/// Clear the sampler texture with a uniform texel
		void clear(texel_type const& Texel);

		/// Sample the sampler texture at a specific level
		texel_type texture_lod(normalized_type const& SampleCoord, size_type layer, size_type Face, level_type Level) const;

		/// Generate all the mipmaps of the sampler texture from the texture base level
		void generate_mipmaps(filter Minification);

		/// Generate the mipmaps of the sampler texture from the texture base level to the texture max level included
		void generate_mipmaps(size_type BaseLayer, size_type MaxLayer, size_type BaseFace, size_type MaxFace, size_type BaseLevel, size_type MaxLevel, filter Minification);

	private:
		typedef typename detail::convert<texture_type, T, P>::func convert_type;
		typedef typename detail::convert<texture_type, T, P>::fetchFunc fetch_type;
		typedef typename detail::convert<texture_type, T, P>::writeFunc write_type;
		typedef typename detail::filterBase<detail::DIMENSION_2D, texture_type, interpolate_type, normalized_type, fetch_type, texel_type>::filterFunc filter_type;

		texture_type Texture;
		convert_type Convert;
		texel_type BorderColor;
		filter_type Filter;
	};

	typedef sampler_cube_array<float> fsamplerCubeArray;
	typedef sampler_cube_array<double> dsamplerCubeArray;
	typedef sampler_cube_array<unsigned int> usamplerCubeArray;
	typedef sampler_cube_array<int> isamplerCubeArray;

}//namespace gli


extern "C" {
    
    namespace bindings {
        
        namespace FSamplerCubeArray {
            
            gli::fsamplerCubeArray fsampler_cube_array_new(const gli::texture_cube_array & Texture, gli::wrap Wrap, gli::filter Mip, gli::filter Min) {
                return gli::fsamplerCubeArray(Texture, Wrap, Mip, Min);
            }
            
            void fsampler_cube_array_set_border_color(gli::fsamplerCubeArray & Sampler, TexelType4F BorderColor) {
                Sampler.set_border_color(BorderColor.into_raw());
            }
            
            void fsampler_cube_array_clear(gli::fsamplerCubeArray & Sampler, TexelType4F Texel) {
                Sampler.clear(Texel.into_raw());
            }
    
            TexelType4F fsampler_cube_array_texel_fetch(const gli::fsamplerCubeArray & Sampler, gli::fsamplerCubeArray::extent_type TexelCoord, gli::texture::size_type Layer, gli::texture::size_type Face, gli::texture::size_type Level) {
                gli::vec4 raw = Sampler.texel_fetch(TexelCoord, Layer, Face, Level);
                return TexelType4F(raw);
            }
            
            void fsampler_cube_array_texel_write(gli::fsamplerCubeArray & Sampler, gli::fsamplerCubeArray::extent_type TexelCoord, gli::texture::size_type Layer, gli::texture::size_type Face, gli::texture::size_type Level, TexelType4F Texel) {
                return Sampler.texel_write(TexelCoord, Layer, Face, Level, Texel.into_raw());
            }
            
            TexelType4F fsampler_cube_array_texel_lod(const gli::fsamplerCubeArray & Sampler, const float SampleCoord[2], gli::texture::size_type Layer, gli::texture::size_type Face, gli::texture::size_type Level) {
                gli::vec4 raw = Sampler.texture_lod(gli::fsamplerCubeArray::normalized_type(SampleCoord[0], SampleCoord[1]), Layer, Face, Level);
                return TexelType4F(raw);
            }
            
            const gli::texture_cube_array & fsampler_cube_array_target_texture(const gli::fsamplerCubeArray & Sampler) {
                return Sampler.operator()();
            }
    
            void fsampler_cube_array_generate_mipmaps1(gli::fsamplerCubeArray & Sampler, gli::filter Minification) {
                Sampler.generate_mipmaps(Minification);
            }
            
            void fsampler_cube_array_generate_mipmaps2(gli::fsamplerCubeArray & Sampler, gli::texture::size_type BaseLayer, gli::texture::size_type MaxLayer, gli::texture::size_type BaseFace, gli::texture::size_type MaxFace, gli::texture::size_type BaseLevel, gli::texture::size_type MaxLevel, gli::filter Minification) {
                Sampler.generate_mipmaps(BaseLayer, MaxLayer, BaseFace, MaxFace, BaseLevel, MaxLevel, Minification);
            }
        }
    }
}

#ifdef GLI_IMPLEMENTATION
#include "sampler_cube_array.inl"
#endif
