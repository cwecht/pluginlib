#include <pluginlib/class_loader.h>
#include "test_base.h"
#include <gtest/gtest.h>

TEST(PluginlibTest, unknownPlugin)
{
  pluginlib::ClassLoader<test_base::Fubar> test_loader("pluginlib", "test_base::Fubar");

  try
  {
    boost::shared_ptr<test_base::Fubar> foo = test_loader.createInstance("pluginlib/foobar");
    foo->initialize(10.0);
  }
  catch(pluginlib::LibraryLoadException& ex)
  {
    SUCCEED();
    return;
  }
  catch(...)
  {
    FAIL() << "Uncaught exception";
  }
  ADD_FAILURE() << "Didn't throw exception as expected";
 
}

TEST(PluginlibTest, misspelledPlugin)
{
  pluginlib::ClassLoader<test_base::Fubar> bad_test_loader("pluginlib", "test_base::Fuba");

  try
  {
    boost::shared_ptr<test_base::Fubar> foo = bad_test_loader.createInstance("pluginlib/foo");
    foo->initialize(10.0);
  }
  catch(pluginlib::LibraryLoadException& ex)
  {
    SUCCEED();
    return;
  }
  catch(...)
  {
    FAIL() << "Uncaught exception";
  }
  ADD_FAILURE() << "Didn't throw exception as expected";
 
}

TEST(PluginlibTest, invalidPackage)
{  
  try
  {
    pluginlib::ClassLoader<test_base::Fubar> bad_test_loader("pluginlib_bad", "test_base::Fubar");
  }
  catch(pluginlib::ClassLoaderException& ex)
  {
    SUCCEED();
    return;
  }
  catch(...)
  {
    FAIL() << "Uncaught exception";
  }
  ADD_FAILURE() << "Didn't throw exception as expected";
 
}

TEST(PluginlibTest, brokenPlugin)
{
  pluginlib::ClassLoader<test_base::Fubar> test_loader("pluginlib", "test_base::Fubar");

  try
  {
    boost::shared_ptr<test_base::Fubar> none = test_loader.createInstance("pluginlib/none");
    none->initialize(10.0);
  }
  catch(pluginlib::PluginlibException& ex)
  {
    SUCCEED();
    return;
  }
  catch(class_loader::ClassLoaderException& ex)
  {
    FAIL() << "class_loader exception instead of pluginlib, argh. " << ex.what() << "\n";
  }
  catch(...)
  {
    FAIL() << "Uncaught exception";
  }
  ADD_FAILURE() << "Didn't throw exception as expected";
 
}

TEST(PluginlibTest, workingPlugin)
{
  pluginlib::ClassLoader<test_base::Fubar> test_loader("pluginlib", "test_base::Fubar");
  
  try
  {
    boost::shared_ptr<test_base::Fubar> foo = test_loader.createInstance("pluginlib/foo");
    foo->initialize(10.0);
    EXPECT_EQ(foo->result(),100.0);

  }
  catch(pluginlib::PluginlibException& ex)
  {
    FAIL() << "Throwing exception: " << ex.what();
    return;
  }
  catch(...)
  {
    FAIL() << "Uncaught exception";
  }
}

TEST(PluginlibTest, createUnmanagedInstanceAndUnloadLibrary)
{
  ROS_INFO( "Making the ClassLoader..." );
  pluginlib::ClassLoader<test_base::Fubar> pl("pluginlib", "test_base::Fubar");

  ROS_INFO( "Instantiating plugin..." );
  test_base::Fubar *inst = pl.createUnmanagedInstance("pluginlib/foo");

  ROS_INFO( "Deleting plugin..." );
  delete inst;

  ROS_INFO( "Checking if plugin is loaded with isClassLoaded..." );
  if( pl.isClassLoaded( "pluginlib/foo" ) )
    ROS_INFO( "Class is loaded" );
  else
  {
    FAIL() <<  "Library containing class should be loaded but isn't.";
  }
  ROS_INFO( "Trying to unload class with unloadLibraryForClass..." );
  try
  {
    pl.unloadLibraryForClass("pluginlib/foo");
  }
  catch(pluginlib::PluginlibException& e)
  {
    FAIL() << "Could not unload library when I should be able to.";
  }
  ROS_INFO( "Done." );
}

TEST(PluginlibTest, createManagedInstanceAndUnloadLibrary)
{
  ROS_INFO( "Making the ClassLoader..." );
  pluginlib::ClassLoader<test_base::Fubar> pl("pluginlib", "test_base::Fubar");

  ROS_INFO( "Instantiating plugin..." );
  {
    boost::shared_ptr<test_base::Fubar> inst = pl.createInstance("pluginlib/foo");
  }

  ROS_INFO( "Checking if plugin is loaded with isClassLoaded..." );
  if( pl.isClassLoaded( "pluginlib/foo" ) )
    ROS_INFO( "Class is loaded" );
  else
  {
    FAIL() <<  "Library containing class should be loaded but isn't.";
  }
  
  ROS_INFO( "Trying to unload class with unloadLibraryForClass..." );
  try
  {
    pl.unloadLibraryForClass("pluginlib/foo");
  }
  catch(pluginlib::PluginlibException& e)
  {
    FAIL() << "Could not unload library when I should be able to.";
  }
  ROS_INFO( "Done." );
}

TEST(PluginlibTest, brokenXML)
{
  try
  {
    pluginlib::ClassLoader<test_base::Fubar> test_loader("pluginlib", "test_base::Fubar", "plugin_test");
  }
  catch(pluginlib::ClassLoaderException& ex)
  {
    SUCCEED();
    return;
  }
  //catch(...)
  //{
  //  FAIL() << "Uncaught exception";
  //}
  ADD_FAILURE() << "Didn't throw exception as expected";

}

// Run all the tests that were declared with TEST()
int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


