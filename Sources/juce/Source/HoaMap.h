/*
  ==============================================================================

    HoaMap.h
    Created: 13 Mar 2013 11:15:41pm
    Author:  eliott PARIS

  ==============================================================================
*/

#ifndef __HoaMap_H_5D25AFEF__
#define __HoaMap_H_5D25AFEF__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Ambisonics/cicmTools.h"

#ifndef HOA_PI
    #define HOA_PI		(3.1415926535897932384626433832795028842)
    #define HOA_2PI		(2. * 3.1415926535897932384626433832795028842)
#endif

#define DrawEllipse(index) g.fillEllipse(m_sources_ordinate[index]-0.04, m_sources_abscissa[index]-0.04, 0.08, 0.08);

//==============================================================================
/*
*/
class HoaMap    :
    public Component,
    public ChangeBroadcaster
{
public:
    HoaMap();
    ~HoaMap();

    void paint (Graphics&);
    void draw_head (Graphics&);
    void draw_speakers (Graphics&);
    void draw_sources (Graphics&);
    void draw_source (Graphics& g, int _sourceIndex);
    void draw_source_in_polar(Graphics& g, int _sourceIndex, float _radius, float _angle);
    void draw_source_in_cart(Graphics& g, int _sourceIndex, float _X, float _Y);
    void draw_source_in_pixel(Graphics& g, int _sourceIndex, float _X, float _Y);
    void resized();
    
    void valueChanged (Value& value);
    
    //mouse events :
    void mouseMove (const MouseEvent &event);
    void mouseDown (const MouseEvent &event);
    void mouseDrag (const MouseEvent &event);
    void mouseUp   (const MouseEvent &event);
    
    // setters :
    int setNbSources(const int _nbSources);
    int setNbSpeakers(int _nbSpeakers);
    int setSpeakerDistance(float _speakerDistance);
    int setSpeakerOffset(float _speakerOffset);
    int setSourceAbscissa(int _sourceIndex, float _newAbscissa, NotificationType notification = sendNotificationAsync);
    int setSourceOrdinate(int _sourceIndex, float _newOrdinate, NotificationType notification = sendNotificationAsync);
    
    // getters :
    int   getNbSources()  { return m_nbSources; }
    int   getNbSpeakers() { return m_nbSpeakers; }
    float getSpeakerDistance() { return m_speakerDistance; }
    float getSpeakerOffset() { return m_speakerOffset; }
    float getSourceOrdinate(int _sourceIndex);
    float getSourceAbscissa(int _sourceIndex);
    
    void triggerChangeMessage (const NotificationType notification);

private:
    int m_minimum_of_loudspeakers, m_maximum_of_loudspeakers, m_minimum_of_sources, m_maximum_of_sources;
    int m_nbSources, m_nbSpeakers;
    float m_speakerOffset, m_speakerDistance;
    float m_sources_ordinate[64], m_sources_abscissa[64];
    //Point<float> *m_sources;
    Point<float> m_sources[64];
    float m_sourceSize;
    int m_sourcePointed, m_sourceOver;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HoaMap)
};


#endif  // __HoaMap_H_5D25AFEF__
