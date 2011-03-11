/* Auto-generated by genmsg_cpp for file /home/student/dev/JKU_Robotic_Stack/Robots/joy/msg/Joy.msg */
#ifndef JOY_MESSAGE_JOY_H
#define JOY_MESSAGE_JOY_H
#include <string>
#include <vector>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/message.h"
#include "ros/time.h"


namespace joy
{
template <class ContainerAllocator>
struct Joy_ : public ros::Message
{
  typedef Joy_<ContainerAllocator> Type;

  Joy_()
  : axes()
  , buttons()
  {
  }

  Joy_(const ContainerAllocator& _alloc)
  : axes(_alloc)
  , buttons(_alloc)
  {
  }

  typedef std::vector<float, typename ContainerAllocator::template rebind<float>::other >  _axes_type;
  std::vector<float, typename ContainerAllocator::template rebind<float>::other >  axes;

  typedef std::vector<int32_t, typename ContainerAllocator::template rebind<int32_t>::other >  _buttons_type;
  std::vector<int32_t, typename ContainerAllocator::template rebind<int32_t>::other >  buttons;


  ROS_DEPRECATED uint32_t get_axes_size() const { return (uint32_t)axes.size(); }
  ROS_DEPRECATED void set_axes_size(uint32_t size) { axes.resize((size_t)size); }
  ROS_DEPRECATED void get_axes_vec(std::vector<float, typename ContainerAllocator::template rebind<float>::other > & vec) const { vec = this->axes; }
  ROS_DEPRECATED void set_axes_vec(const std::vector<float, typename ContainerAllocator::template rebind<float>::other > & vec) { this->axes = vec; }
  ROS_DEPRECATED uint32_t get_buttons_size() const { return (uint32_t)buttons.size(); }
  ROS_DEPRECATED void set_buttons_size(uint32_t size) { buttons.resize((size_t)size); }
  ROS_DEPRECATED void get_buttons_vec(std::vector<int32_t, typename ContainerAllocator::template rebind<int32_t>::other > & vec) const { vec = this->buttons; }
  ROS_DEPRECATED void set_buttons_vec(const std::vector<int32_t, typename ContainerAllocator::template rebind<int32_t>::other > & vec) { this->buttons = vec; }
private:
  static const char* __s_getDataType_() { return "joy/Joy"; }
public:
  ROS_DEPRECATED static const std::string __s_getDataType() { return __s_getDataType_(); }

  ROS_DEPRECATED const std::string __getDataType() const { return __s_getDataType_(); }

private:
  static const char* __s_getMD5Sum_() { return "e3ef016fcdf22397038b36036c66f7c8"; }
public:
  ROS_DEPRECATED static const std::string __s_getMD5Sum() { return __s_getMD5Sum_(); }

  ROS_DEPRECATED const std::string __getMD5Sum() const { return __s_getMD5Sum_(); }

private:
  static const char* __s_getMessageDefinition_() { return "float32[] axes\n\
int32[] buttons\n\
\n\
"; }
public:
  ROS_DEPRECATED static const std::string __s_getMessageDefinition() { return __s_getMessageDefinition_(); }

  ROS_DEPRECATED const std::string __getMessageDefinition() const { return __s_getMessageDefinition_(); }

  ROS_DEPRECATED virtual uint8_t *serialize(uint8_t *write_ptr, uint32_t seq) const
  {
    ros::serialization::OStream stream(write_ptr, 1000000000);
    ros::serialization::serialize(stream, axes);
    ros::serialization::serialize(stream, buttons);
    return stream.getData();
  }

  ROS_DEPRECATED virtual uint8_t *deserialize(uint8_t *read_ptr)
  {
    ros::serialization::IStream stream(read_ptr, 1000000000);
    ros::serialization::deserialize(stream, axes);
    ros::serialization::deserialize(stream, buttons);
    return stream.getData();
  }

  ROS_DEPRECATED virtual uint32_t serializationLength() const
  {
    uint32_t size = 0;
    size += ros::serialization::serializationLength(axes);
    size += ros::serialization::serializationLength(buttons);
    return size;
  }

  typedef boost::shared_ptr< ::joy::Joy_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::joy::Joy_<ContainerAllocator>  const> ConstPtr;
}; // struct Joy
typedef  ::joy::Joy_<std::allocator<void> > Joy;

typedef boost::shared_ptr< ::joy::Joy> JoyPtr;
typedef boost::shared_ptr< ::joy::Joy const> JoyConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::joy::Joy_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::joy::Joy_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace joy

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator>
struct MD5Sum< ::joy::Joy_<ContainerAllocator> > {
  static const char* value() 
  {
    return "e3ef016fcdf22397038b36036c66f7c8";
  }

  static const char* value(const  ::joy::Joy_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xe3ef016fcdf22397ULL;
  static const uint64_t static_value2 = 0x038b36036c66f7c8ULL;
};

template<class ContainerAllocator>
struct DataType< ::joy::Joy_<ContainerAllocator> > {
  static const char* value() 
  {
    return "joy/Joy";
  }

  static const char* value(const  ::joy::Joy_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::joy::Joy_<ContainerAllocator> > {
  static const char* value() 
  {
    return "float32[] axes\n\
int32[] buttons\n\
\n\
";
  }

  static const char* value(const  ::joy::Joy_<ContainerAllocator> &) { return value(); } 
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::joy::Joy_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.axes);
    stream.next(m.buttons);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct Joy_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::joy::Joy_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::joy::Joy_<ContainerAllocator> & v) 
  {
    s << indent << "axes[]" << std::endl;
    for (size_t i = 0; i < v.axes.size(); ++i)
    {
      s << indent << "  axes[" << i << "]: ";
      Printer<float>::stream(s, indent + "  ", v.axes[i]);
    }
    s << indent << "buttons[]" << std::endl;
    for (size_t i = 0; i < v.buttons.size(); ++i)
    {
      s << indent << "  buttons[" << i << "]: ";
      Printer<int32_t>::stream(s, indent + "  ", v.buttons[i]);
    }
  }
};


} // namespace message_operations
} // namespace ros

#endif // JOY_MESSAGE_JOY_H

