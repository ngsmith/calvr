/**
 * @file PluginManager.h
 */

#ifndef CALVR_PLUGIN_MANAGER_H
#define CALVR_PLUGIN_MANAGER_H

#include <kernel/CVRPlugin.h>

#include <string>
#include <vector>
//#include <queue>
#include <map>

namespace cvr
{

/*template< typename FirstType, typename SecondType >
 struct PrioritySort
 {
 bool operator() (std::pair<FirstType,SecondType> const& first, std::pair<FirstType,SecondType> const& second)
 {
 return first.first > second.first;
 }
 };*/

struct InteractionEvent;

/**
 * @brief Loads and manager CalVR plugins
 */
class PluginManager
{
    public:
        ~PluginManager();

        /**
         * @brief Get static pointer to class instance
         */
        static PluginManager * instance();

        /**
         * @brief Load plugins specified in config file
         */
        bool init();

        /**
         * @brief Do preFrame callback on all loaded plugins
         */
        void preFrame();

        /**
         * @brief Do postFrame callback on all loaded plugins
         */
        void postFrame();

        /**
         * @brief Forward interaction event to loaded plugins
         */
        bool processEvent(InteractionEvent * event);

        void sendMessageByName(std::string plugin, int type, char * data);

    protected:
        PluginManager();

        static PluginManager * _myPtr; ///< static self pointer

        /**
         * @brief Open a plugin from a shared library by name
         */
        bool loadPlugin(std::string plugin);

        /**
         * @brief Structure with info for a loaded plugin
         */
        struct PluginInfo
        {
                int priority;       ///< plugin priority
                CVRPlugin * ptr;    ///< pointer to instance of loaded plugin
                std::string name;   ///< name of plugin
        };

        /**
         * @brief Contains function used to sort vector of plugins based on priority
         */
        struct PrioritySort
        {
                bool operator()(PluginInfo* const & first,
                                PluginInfo* const & second)
                {
                    return first->priority > second->priority;
                }
        };

        std::string _pluginLibDir; ///< directory containing plugin shared libraries
        //std::priority_queue<std::pair<int,CVRPlugin*>,std::vector<std::pair<int,CVRPlugin*> >,PrioritySort<int,CVRPlugin*> > _pluginList;
        std::vector<PluginInfo *> _loadedPluginList; ///< list of loaded plugins
        std::map<std::string,bool> _pluginMap; ///< map containing all plugin names in the config file and if they are on

};

}

#endif