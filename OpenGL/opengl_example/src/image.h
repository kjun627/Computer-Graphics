#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "common.h"

CLASS_PTR(Image)
class Image{
public:
    static ImageUPtr Load(const std::string& filepath);
    static ImageUPtr Create(int width, int height, int channelCount = 4);
    ~Image();

    const uint8_t* GetData() const {
        return m_data;
    }
    int GetWidth() const {
        return m_width;
    }
    int GetHight() const {
        return m_height;
    }
    int GetChannelCount() const {
        return m_channelCount;
    }

    void SetCheckImage(int gridx, int gridY);
private:
    Image() {};
    bool LoadWithStb(const std::string& filepath);
    bool Allocatie(int width, int geight, int channelCount);
    int m_width{0};
    int m_height{0};
    int m_channelCount {0};
    uint8_t* m_data{nullptr};
};
#endif // __IMAGE_H__