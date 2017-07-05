#include "stdafx.h"
#include "r_imagedecode.h"

struct WaveletDecode
{
	unsigned __int16 value;
	unsigned __int16 bit;
	const char *data;
	int width;
	int height;
	int channels;
	int bpp;
	int mipLevel;
	bool dataInitialized;
};

struct WaveletHuffmanDecode
{
	__int16 value;
	__int16 bits;
};

static WaveletHuffmanDecode* waveletDecodeBlue = (WaveletHuffmanDecode*)0x0044E370;
static WaveletHuffmanDecode* waveletDecodeRedGreen = (WaveletHuffmanDecode*)0x00452370;
static WaveletHuffmanDecode* waveletDecodeAlpha = (WaveletHuffmanDecode*)0x00456370;

static void Wavelet_ConsumeBits(unsigned __int16 bitCount, WaveletDecode *decode)
{
	ASSERT(bitCount > 0 && bitCount <= 16);
	ASSERT(decode->bit < 8);

	decode->value >>= bitCount;

	decode->value |= (((BYTE)decode->data[0] << 0)	|
					  ((BYTE)decode->data[1] << 8)	|
					  ((BYTE)decode->data[2] << 16)	|
					  ((BYTE)decode->data[3] << 24)) >> decode->bit << (16 - bitCount);

	decode->bit += bitCount;
	decode->data += decode->bit >> 3;
	decode->bit &= 7u;
}

static int Wavelet_DecodeValue(WaveletHuffmanDecode *decodeTable, unsigned __int16 bitCount, int bias, WaveletDecode *decode)
{
	ASSERT((1 << bitCount) - 1 >= bias * 2 - 1);
	ASSERT((1 << (bitCount - 1)) - 1 < bias * 2 - 1);
	
	int index = decode->value & 0xFFF;
	Wavelet_ConsumeBits(decodeTable[index].bits, decode);
	int value = decodeTable[index].value;
	
	if (value == 0xFFFF8000)
	{
		value = (((1 << bitCount) - 1) & decode->value) - bias;
		Wavelet_ConsumeBits(bitCount, decode);
	}
	
	return value;
}

static void Wavelet_AddDeltaToMipmap(char *inout, int size, WaveletDecode *decode, const int *dstChanOffset)
{
	for (int i = 0; i < size; i++)
	{
		for (int chanIndex = 0; chanIndex < decode->channels; chanIndex++)
		{
			char* value = &inout[dstChanOffset[chanIndex]];
			char old = *value;
			*value = Wavelet_DecodeValue(waveletDecodeAlpha, 9u, 255, decode) + old;
		}

		inout += decode->bpp;
	}
}

static void Wavelet_DecompressLevel(char *src, char *dst, WaveletDecode *decode)
{
	ASSERT_MSG_VA(decode->bpp >= 1 && decode->bpp <= 4, "decode->bpp not in [1, 4]\n\t%i not in [%i, %i]", decode->bpp, 1, 4);

	ASSERT(decode->bpp == decode->channels || (decode->bpp == 4 && decode->channels == 3));
	ASSERT(decode->mipLevel >= 0);
	
	int dstChanOffset[4];
	int dstBpp = decode->bpp;

	switch (dstBpp)
	{
	case 1:
		dstChanOffset[0] = 0;
		break;
	case 2:
		dstChanOffset[0] = 0;
		dstChanOffset[1] = 1;
		break;
	case 3:
	case 4:
		dstChanOffset[0] = 0;
		dstChanOffset[1] = 1;
		dstChanOffset[2] = 2;
		dstChanOffset[3] = 3;
		break;
	default:
		break;
	}

	int w = decode->width >> decode->mipLevel;
	int h = decode->height >> decode->mipLevel;

	if (w > 1 && h > 1)
	{
		if (!decode->dataInitialized)
		{
			decode->value = (((BYTE)decode->data[1]) << 8) | (BYTE)decode->data[0];
			decode->bit = 0;
			decode->data += 2;
			decode->dataInitialized = 1;
		}

		bool needsMipDelta = (decode->value & 1) != 0;
		Wavelet_ConsumeBits(1, decode);

		if (needsMipDelta)
			Wavelet_AddDeltaToMipmap(src, h * w / 4, decode, dstChanOffset);

		int stride = dstBpp * w;

		int coeff[4][3];
		for (int y = 0; y < h; y += 2)
		{
			for (int x = 0; x < w; x += 2)
			{
				ASSERT(dst + stride + dstBpp <= src || dst > src);

				if (decode->channels != 1)
				{
					int evenOddParity = decode->value & 1;
					Wavelet_ConsumeBits(1, decode);

					coeff[0][0] = Wavelet_DecodeValue(waveletDecodeBlue, 9, 255, decode);
					coeff[0][1] = Wavelet_DecodeValue(waveletDecodeBlue, 9, 255, decode);
					coeff[0][2] = Wavelet_DecodeValue(waveletDecodeBlue, 9, 255, decode);

					int base = 2 * (BYTE)src[dstChanOffset[0]];
					char* dstChan = &dst[dstChanOffset[0]];

					dst[dstChanOffset[0]] = evenOddParity + ((coeff[0][2] + coeff[0][1] + coeff[0][0] + base) >> 1);

					dstChan[dstBpp] = (coeff[0][0] + base - (coeff[0][2] + coeff[0][1])) >> 1;
					dstChan[stride] = (coeff[0][1] - coeff[0][2] + base - coeff[0][0]) >> 1;
					dstChan[dstBpp + stride] = (base - coeff[0][0] - (coeff[0][1] - coeff[0][2])) >> 1;

					if (decode->channels >= 3)
					{
						evenOddParity = decode->value & 1;
						Wavelet_ConsumeBits(1, decode);

						coeff[1][0] = coeff[0][0] + Wavelet_DecodeValue(waveletDecodeRedGreen, 10, 510, decode);
						coeff[1][1] = coeff[0][1] + Wavelet_DecodeValue(waveletDecodeRedGreen, 10, 510, decode);
						coeff[1][2] = coeff[0][2] + Wavelet_DecodeValue(waveletDecodeRedGreen, 10, 510, decode);

						base = 2 * (BYTE)src[dstChanOffset[1]];
						dstChan = &dst[dstChanOffset[1]];

						dst[dstChanOffset[1]] = evenOddParity + ((coeff[1][2] + coeff[1][1] + coeff[1][0] + base) >> 1);

						dstChan[dstBpp] = (coeff[1][0] + base - (coeff[1][2] + coeff[1][1])) >> 1;
						dstChan[stride] = (coeff[1][1] - coeff[1][2] + base - coeff[1][0]) >> 1;
						dstChan[dstBpp + stride] = (base - coeff[1][0] - (coeff[1][1] - coeff[1][2])) >> 1;

						evenOddParity = decode->value & 1;
						Wavelet_ConsumeBits(1, decode);
						
						coeff[2][0] = coeff[0][0] + Wavelet_DecodeValue(waveletDecodeRedGreen, 10, 510, decode);
						coeff[2][1] = coeff[0][1] + Wavelet_DecodeValue(waveletDecodeRedGreen, 10, 510, decode);
						coeff[2][2] = coeff[0][2] + Wavelet_DecodeValue(waveletDecodeRedGreen, 10, 510, decode);
						
						base = 2 * (BYTE)src[dstChanOffset[2]];
						dstChan = &dst[dstChanOffset[2]];

						dst[dstChanOffset[2]] = evenOddParity + ((coeff[2][2] + coeff[2][1] + coeff[2][0] + base) >> 1);

						dstChan[dstBpp] = (coeff[2][0] + base - (coeff[2][2] + coeff[2][1])) >> 1;
						dstChan[stride] = (coeff[2][1] - coeff[2][2] + base - coeff[2][0]) >> 1;
						dstChan[dstBpp + stride] = (base - coeff[2][0] - (coeff[2][1] - coeff[2][2])) >> 1;
					}
				}

				if (decode->channels == 3)
				{
					if (decode->channels != decode->bpp)
					{
						char* dstChan = &dst[dstChanOffset[3]];

						dst[dstChanOffset[3]] = -1;

						dstChan[dstBpp] = -1;
						dstChan[stride] = -1;
						dstChan[dstBpp + stride] = -1;
					}
				}
				else
				{
					int evenOddParity = decode->value & 1;
					Wavelet_ConsumeBits(1, decode);

					coeff[3][0] = Wavelet_DecodeValue(waveletDecodeAlpha, 9, 255, decode);
					coeff[3][1] = Wavelet_DecodeValue(waveletDecodeAlpha, 9, 255, decode);
					coeff[3][2] = Wavelet_DecodeValue(waveletDecodeAlpha, 9, 255, decode);

					int base = 2 * src[dstChanOffset[decode->channels - 1]];
					char* dstChan = &dst[dstChanOffset[decode->channels - 1]];

					dstChan[0] = evenOddParity + ((coeff[3][2] + coeff[3][1] + coeff[3][0] + base) >> 1);

					dstChan[dstBpp] = (coeff[3][0] + base - (coeff[3][2] + coeff[3][1])) >> 1;
					dstChan[stride] = (coeff[3][1] - coeff[3][2] + base - coeff[3][0]) >> 1;
					dstChan[dstBpp + stride] = (base - coeff[3][0] - (coeff[3][1] - coeff[3][2])) >> 1;
				}

				src += dstBpp;
				dst += 2 * dstBpp;
			}

			dst += stride;
		}
	}
	else
	{
		w = max(w, 1);
		h = max(h, 1);

		int size = h * w;
		ASSERT(size >= 1);

		for (; size > 0; size--)
		{
			for (int chanIndex = 0; chanIndex < decode->channels; ++chanIndex)
				dst[dstChanOffset[chanIndex]] = *decode->data++;

			if (decode->bpp != decode->channels)
				dst[dstChanOffset[3]] = -1;

			dst += dstBpp;
		}
	}
}

void __cdecl Image_DecodeWavelet(GfxRawImage *image, t5::GfxImageFileHeader *imageFile, unsigned __int8 *data, int bytesPerPixel)
{
	PBYTE to[6];
	PBYTE from[6];

	PBYTE pixels[6];

	ASSERT(image);
	ASSERT(imageFile);

	WaveletDecode decode;
	decode.width = imageFile->dimensions[0];
	decode.height = imageFile->dimensions[1];
	decode.value = 0;
	decode.bit = 0;

	int mipCount = Image_CountMipmapsForFile(imageFile);
	int height = imageFile->dimensions[1];
	int width = imageFile->dimensions[0];

	decode.mipLevel = mipCount - 1;
	decode.channels = bytesPerPixel;
	decode.bpp = bytesPerPixel;
	decode.dataInitialized = 0;

	unsigned int totalSize = bytesPerPixel * width * height;
	int faceCount = ((imageFile->flags & t5::IMG_FLAG_CUBEMAP) != 0) ? 6 : 1;

	memset(to, 0, sizeof(PBYTE) * faceCount);
	for (int faceIndex = 0; faceIndex < faceCount; faceIndex++)
	{
		pixels[faceIndex++] = (PBYTE)_Hunk_AllocTempMemoryInternal(totalSize);
	}

	unsigned __int8 mipLevel = decode.mipLevel;
	for (decode.data = (const char *)data; decode.mipLevel >= 0; mipLevel = --decode.mipLevel)
	{
		int mipWidth = max(decode.width >> mipLevel, 1);
		int mipHeight = max(decode.height >> mipLevel, 1);
	
		unsigned int mipSize = bytesPerPixel * mipWidth * mipHeight;
	
		for (int faceIndex = 0; faceIndex < faceCount; faceIndex++)
		{
			from[faceIndex] = to[faceIndex];
			to[faceIndex] = &pixels[faceIndex][totalSize] - mipSize;
	
			Wavelet_DecompressLevel((char*)from[faceIndex], (char*)to[faceIndex], &decode);
			if (faceIndex == 0 && decode.mipLevel == 0)
				Image_CopyBitmapData(to[faceIndex], image, imageFile);
		}
	}

	for (int i = faceCount - 1; i >= 0; --i)
	{
		_Hunk_FreeTempMemory(pixels[i]);
	}
}
