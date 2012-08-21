/**
 * @file TrackeriOmnicron.h
 */
#ifndef CALVR_TRACKER_KINECT_H
#define CALVR_TRACKER_KINECT_H

#include <zmq.hpp>
#include "shared/PubSub.h"
#include "protocol/skeletonframe.pb.h"

#include <cvrInput/TrackerBase.h>

#include <vector>
#include <map>

namespace cvr
{

/**
 * @addtogroup input
 * @{
 */

/**
 * @brief Tracker that runs on the slave nodes
 */
class TrackerKinect : public TrackerBase
{
    public:
        TrackerKinect();
        virtual ~TrackerKinect();

        virtual bool init(std::string tag);

        virtual TrackedBody * getBody(int index);
        virtual unsigned int getButtonMask();
        virtual float getValuator(int index);

        virtual int getNumBodies();
        virtual int getNumValuators();
        virtual int getNumButtons();

        virtual bool thread()
        {
            return true;
        }

        virtual TrackerType getTrackerType()
        {
            return TRACKER;
        }

        virtual void update(
                std::map<int,std::list<InteractionEvent*> > & eventMap);

//        virtual void onEvent(const omicronConnector::EventData& e);
    protected:
        zmq::context_t* context;// = zmq::context_t(1);
        SubSocket<RemoteKinect::SkeletonFrame>* skel_socket;
//        omicronConnector::OmicronConnectorClient * _client;

        int _numBodies; ///< number of bodies
        int _numButtons; ///< number of buttons
        int _numVals; ///< number of valuators

        std::map<unsigned int,std::vector<TrackedBody*> > _bodyMap;

        unsigned int _buttonMask; ///< current button mask
        float * _valArray; ///< array of valuator values
};

/**
 * @}
 */

}

#endif
