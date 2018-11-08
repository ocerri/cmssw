#ifndef DECAYTREEFITTER_CONFIG_H
#define DECAYTREEFITTER_CONFIG_H

#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

namespace DecayTreeFitter
{
  class Configuration
  {
  public:
  Configuration(edm::ESHandle<TransientTrackBuilder> theTTBuilder,
                edm::ESHandle<MagneticField> bFieldHandle,
                bool forceFitAll=true
               )
    : m_forceFitAll(forceFitAll),
      m_TTBuilder(theTTBuilder),
      m_MagneticField(bFieldHandle),
      m_useTrackTraj( true )
	{}

    const edm::ESHandle<TransientTrackBuilder> transientTrackBuilder() const { return m_TTBuilder ; }

    bool forceFitAll() const { return m_forceFitAll ; }
    bool useTrackTraj() const { return m_useTrackTraj ; }

  private:
    bool m_forceFitAll ;
    const edm::ESHandle<TransientTrackBuilder> m_TTBuilder ;
    const edm::ESHandle<MagneticField> m_MagneticField ;
    bool m_useTrackTraj ;
  } ;
}

#endif
