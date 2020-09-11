#pragma once
#include "m2Texture.h"
#include <array>
#include <thread>
class m2StreamingDemo
{
public:
	m2StreamingDemo();
	~m2StreamingDemo();

	void update();
	void render();

private:
	//8k textures; performance decreases 35ms per frame per additional texture.
	//4k textures; performance decreases 10ms per frame per additional texture.
	//1k textures; performance decreases 3ms  per frame per additional texture.

	//8k texture; performance decreases 13ms per CPU update per additional texture.
	//4k texture; performance decreases  3ms per CPU update per additional texture.
	//1k texture; performance decreases ~1ms per CPU update per additional texture.

	//8k texture; performance decreases 23  ms per GPU update per additional texture.
	//4k texture; performance decreases  6  ms per GPU update per additional texture.
	//1k texture; performance decreases ~1.3ms per GPU update per additional texture.

	//This means bandwidth is maxed out which is good. Thread dispatch and PBO dispatch are asynchronous no matter what so our code is behaving as expected.
	//Moreover its to measure with integers for 1k textures. Non-constant growth makes sense in this case because we probably haven't yet maxed out bandwidth.
	std::array<m2Texture,   5> m_textures;
	std::array<std::thread, 5> m_threads;
};