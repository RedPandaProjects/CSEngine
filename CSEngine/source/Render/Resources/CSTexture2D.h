#pragma once
class CSTexture2D :public CSResourceNamed
{
	BEAR_CLASS_WITHOUT_COPY(CSTexture2D);
public:
	CSTexture2D(BearStringConteniar Name);
	CSTexture2D(BearStringConteniar Name, void* data, bsize size);

	~CSTexture2D();
	BearFactoryPointer<BearRHI::BearRHITexture2D> Texture2D;
	inline const BearVector2<bsize>& GetSize()const {
		return m_Size;
	}
	inline bool Empty()const { return m_empty; }
private:
	bool m_empty;
	BearVector2<bsize> m_Size;
};
