#ifndef EVENT
#define EVENT

#include "isosegment.h"
#include <memory>

enum EventType { START_EVENT, END_EVENT, VERTICAL };

class Event {

public:
  Event(const int _x,
        const EventType _eventType,
        const std::shared_ptr<IsoSegment> _isoSeg):
        x(_x),
        eventType(_eventType),
        isoSeg(_isoSeg) {}

  // Die X-Koordinate eines Events ist der Key
  int x;
  EventType eventType;
  std::shared_ptr<IsoSegment> isoSeg;

  // Kleiner-Ordnung
  bool operator < (const Event &e) const
  {
      return x < e.x;
  }
};

#endif // EVENT
