using GameReaderCommon;
using SimHub.Plugins;
using System;

namespace User.PluginSdkDemo
{



    [PluginDescription("My plugin description")]
    [PluginAuthor("Author")]
    [PluginName("Demo plugin")]
    public class DataPluginDemo : IPlugin, IDataPlugin, IWPFSettings
    {

        public DataPluginDemoSettings Settings;


        /// <summary>
        /// Instance of the current plugin manager
        /// </summary>
        public PluginManager PluginManager { get; set; }

        /// <summary>
        /// Called one time per game data update, contains all normalized game data, 
        /// raw data are intentionnally "hidden" under a generic object type (A plugin SHOULD NOT USE IT)
        /// 
        /// This method is on the critical path, it must execute as fast as possible and avoid throwing any error
        /// 
        /// </summary>
        /// <param name="pluginManager"></param>
        /// <param name="data"></param>
        public void DataUpdate(PluginManager pluginManager, ref GameData data)
        {
            // Define the value of our property (declared in init)
            pluginManager.SetPropertyValue("CurrentDateTime", this.GetType(), DateTime.Now);

            if (data.GameRunning)
            {
                if (data.OldData != null && data.NewData != null)
                {
                    if (data.OldData.SpeedKmh < Settings.SpeedWarningLevel && data.OldData.SpeedKmh >= Settings.SpeedWarningLevel)
                    {
                        // Trigger an event
                        pluginManager.TriggerEvent("SpeedWarning", this.GetType());
                    }
                }
            }
        }

        /// <summary>
        /// Called at plugin manager stop, close/dispose anything needed here ! 
        /// Plugins are rebuilt at game change
        /// </summary>
        /// <param name="pluginManager"></param>
        public void End(PluginManager pluginManager)
        {
            // Save settings
            this.SaveCommonSettings("GeneralSettings", Settings);
        }

        /// <summary>
        /// Returns the settings control, return null if no settings control is required
        /// </summary>
        /// <param name="pluginManager"></param>
        /// <returns></returns>
        public System.Windows.Controls.Control GetWPFSettingsControl(PluginManager pluginManager)
        {
            return new SettingsControlDemo(this);
        }

        /// <summary>
        /// Called once after plugins startup
        /// Plugins are rebuilt at game change
        /// </summary>
        /// <param name="pluginManager"></param>
        public void Init(PluginManager pluginManager)
        {

            SimHub.Logging.Current.Info("Starting plugin");


            // Load settings
            Settings = this.ReadCommonSettings<DataPluginDemoSettings>("GeneralSettings", () => new DataPluginDemoSettings());


            // Declare a property available in the property list
            pluginManager.AddProperty("CurrentDateTime", this.GetType(), DateTime.Now);

            // Declare an event 
            pluginManager.AddEvent("SpeedWarning", this.GetType());

            // Declare an action which can be called
            pluginManager.AddAction("IncrementSpeedWarning", this.GetType(), (a, b) =>
            {
                Settings.SpeedWarningLevel++;
                SimHub.Logging.Current.Info("Speed warning changed");
            });

            // Declare an action which can be called
            pluginManager.AddAction("DecrementSpeedWarning", this.GetType(), (a, b) =>
            {
                Settings.SpeedWarningLevel--;
            });
        }
    }
}