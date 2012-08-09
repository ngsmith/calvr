#include <cvrInput/TrackerKinect.h>

#include <cstring>
#include <iostream>

using namespace cvr;

TrackerKinect::TrackerKinect()
{
}

TrackerKinect::~TrackerKinect()
{
    delete skel_socket;
}

bool TrackerKinect::init(std::string tag)
{
    printf("--- CalVR initializing TrackerKinect\n");
    context = new zmq::context_t(1);
    skel_socket = new SubSocket<RemoteKinect::SkeletonFrame>(*context,"tcp://localhost:9002");
    return true;
}

//
TrackerBase::TrackedBody * TrackerKinect::getBody(int index)
{
std::map<unsigned int,std::vector<TrackedBody*> >::iterator it = _bodyMap.begin();

if (index > (it->second.size()-1)) return NULL;

return it->second[index];
/*    std::map<unsigned int,std::vector<TrackedBody*> >::iterator it = _bodyMap.begin();
    for(int i = 0; i < index; i++)
    {
        ++it;
    }
    return it->second[0];*/
}
//
unsigned int TrackerKinect::getButtonMask()
{
    return 0;
}

float TrackerKinect::getValuator(int index)
{
    return 0.0f;
}

int TrackerKinect::getNumBodies()
{
    int size = 0;
    for(std::map<unsigned int,std::vector<TrackedBody*> >::iterator it = _bodyMap.begin(); it != _bodyMap.end(); it++)
    {
        size++;//= it->second.size();
    }
    return 2*size;
 //   return 6;
}

int TrackerKinect::getNumValuators()
{
    return 0;
}

int TrackerKinect::getNumButtons()
{
    return 0;
}
//
void TrackerKinect::update(
    std::map<int,std::list<InteractionEvent*> > & eventMap)
{

    RemoteKinect::SkeletonFrame sf;
    if (skel_socket->recv(sf))
    {
        for (int i=0;i<sf.skeletons_size();i++)
        {
            int skelid = sf.skeletons(i).skeleton_id();
            if (_bodyMap.find(skelid) == _bodyMap.end())
            {
                TrackedBody* tb = new TrackedBody;
                tb->x = tb->y = tb->z = 0.0;
                osg::Quat q;
                tb->qx = q.x();
                tb->qy = q.y();
                tb->qz = q.z();
                tb->qw = q.w();
                TrackedBody* tb2 = new TrackedBody;
                tb->x = tb->y = tb->z = 0.0;
                tb->qx = q.x();
                tb->qy = q.y();
                tb->qz = q.z();
                tb->qw = q.w();
                _bodyMap[skelid].push_back(tb);
                _bodyMap[skelid].push_back(tb2);
            }

            for (int j=0; j<sf.skeletons(i).joints_size(); j++)
            {
                // head only
                if (sf.skeletons(i).joints(j).type() == 1)
                {
                    _bodyMap[skelid][0]->x = sf.skeletons(i).joints(j).x();
                    _bodyMap[skelid][0]->y = sf.skeletons(i).joints(j).y();
                    _bodyMap[skelid][0]->z = sf.skeletons(i).joints(j).z();            
        
                    _bodyMap[skelid][0]->qx = sf.skeletons(i).joints(j).qx();
                    _bodyMap[skelid][0]->qy = sf.skeletons(i).joints(j).qy();
                    _bodyMap[skelid][0]->qz = sf.skeletons(i).joints(j).qz();
                    _bodyMap[skelid][0]->qw = sf.skeletons(i).joints(j).qw();
                }
                
               if (sf.skeletons(i).joints(j).type() == 9)
                {
                    _bodyMap[skelid][1]->x = sf.skeletons(i).joints(j).x();
                    _bodyMap[skelid][1]->y = sf.skeletons(i).joints(j).y();
                    _bodyMap[skelid][1]->z = sf.skeletons(i).joints(j).z();            
        
                    _bodyMap[skelid][1]->qx = sf.skeletons(i).joints(j).qx();
                    _bodyMap[skelid][1]->qy = sf.skeletons(i).joints(j).qy();
                    _bodyMap[skelid][1]->qz = sf.skeletons(i).joints(j).qz();
                    _bodyMap[skelid][1]->qw = sf.skeletons(i).joints(j).qw();
                }




            }
        }
    }
}
//        if(e.type == omicron::EventBase::Untrace && _bodyMap.find(e.sourceId) != _bodyMap.end())
//        {
//            std::cerr << "Removing bodies for source id: " << e.sourceId << std::endl;
//            for(int i = 0; i < _bodyMap[e.sourceId].size(); i++)
//            {
//                delete _bodyMap[e.sourceId][i];
//            }
//            _bodyMap.erase(e.sourceId);
