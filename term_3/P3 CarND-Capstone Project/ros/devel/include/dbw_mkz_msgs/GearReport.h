// Generated by gencpp from file dbw_mkz_msgs/GearReport.msg
// DO NOT EDIT!


#ifndef DBW_MKZ_MSGS_MESSAGE_GEARREPORT_H
#define DBW_MKZ_MSGS_MESSAGE_GEARREPORT_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>
#include <dbw_mkz_msgs/Gear.h>
#include <dbw_mkz_msgs/Gear.h>
#include <dbw_mkz_msgs/GearReject.h>

namespace dbw_mkz_msgs
{
template <class ContainerAllocator>
struct GearReport_
{
  typedef GearReport_<ContainerAllocator> Type;

  GearReport_()
    : header()
    , state()
    , cmd()
    , reject()
    , override(false)
    , fault_bus(false)  {
    }
  GearReport_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , state(_alloc)
    , cmd(_alloc)
    , reject(_alloc)
    , override(false)
    , fault_bus(false)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef  ::dbw_mkz_msgs::Gear_<ContainerAllocator>  _state_type;
  _state_type state;

   typedef  ::dbw_mkz_msgs::Gear_<ContainerAllocator>  _cmd_type;
  _cmd_type cmd;

   typedef  ::dbw_mkz_msgs::GearReject_<ContainerAllocator>  _reject_type;
  _reject_type reject;

   typedef uint8_t _override_type;
  _override_type override;

   typedef uint8_t _fault_bus_type;
  _fault_bus_type fault_bus;





  typedef boost::shared_ptr< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> const> ConstPtr;

}; // struct GearReport_

typedef ::dbw_mkz_msgs::GearReport_<std::allocator<void> > GearReport;

typedef boost::shared_ptr< ::dbw_mkz_msgs::GearReport > GearReportPtr;
typedef boost::shared_ptr< ::dbw_mkz_msgs::GearReport const> GearReportConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::dbw_mkz_msgs::GearReport_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace dbw_mkz_msgs

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'dbw_mkz_msgs': ['/media/elffer/Samsung_T5/projects/autonomous_driving/codes/sdce/term_3/P3 CarND-Capstone Project/ros/src/dbw_mkz_msgs/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> >
{
  static const char* value()
  {
    return "785b94d5bfee677e7f0da982153f2711";
  }

  static const char* value(const ::dbw_mkz_msgs::GearReport_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x785b94d5bfee677eULL;
  static const uint64_t static_value2 = 0x7f0da982153f2711ULL;
};

template<class ContainerAllocator>
struct DataType< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> >
{
  static const char* value()
  {
    return "dbw_mkz_msgs/GearReport";
  }

  static const char* value(const ::dbw_mkz_msgs::GearReport_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Header header\n\
\n\
# Current gear enumeration\n\
Gear state\n\
\n\
# Gear command enumeration\n\
Gear cmd\n\
\n\
# Gear reject enumeration\n\
GearReject reject\n\
\n\
# Status\n\
bool override\n\
\n\
# Faults\n\
bool fault_bus\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n\
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
\n\
================================================================================\n\
MSG: dbw_mkz_msgs/Gear\n\
uint8 gear\n\
\n\
uint8 NONE=0\n\
uint8 PARK=1\n\
uint8 REVERSE=2\n\
uint8 NEUTRAL=3\n\
uint8 DRIVE=4\n\
uint8 LOW=5\n\
\n\
================================================================================\n\
MSG: dbw_mkz_msgs/GearReject\n\
uint8 value\n\
\n\
uint8 NONE=0              # Not rejected\n\
uint8 SHIFT_IN_PROGRESS=1 # Shift in progress\n\
uint8 OVERRIDE=2          # Override on brake, throttle, or steering\n\
uint8 ROTARY_LOW=3        # Rotary shifter can't shift to Low\n\
uint8 ROTARY_PARK=4       # Rotary shifter can't shift out of Park\n\
uint8 VEHICLE=5           # Rejected by vehicle (try pressing the brakes)\n\
";
  }

  static const char* value(const ::dbw_mkz_msgs::GearReport_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.state);
      stream.next(m.cmd);
      stream.next(m.reject);
      stream.next(m.override);
      stream.next(m.fault_bus);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct GearReport_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::dbw_mkz_msgs::GearReport_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::dbw_mkz_msgs::GearReport_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "state: ";
    s << std::endl;
    Printer< ::dbw_mkz_msgs::Gear_<ContainerAllocator> >::stream(s, indent + "  ", v.state);
    s << indent << "cmd: ";
    s << std::endl;
    Printer< ::dbw_mkz_msgs::Gear_<ContainerAllocator> >::stream(s, indent + "  ", v.cmd);
    s << indent << "reject: ";
    s << std::endl;
    Printer< ::dbw_mkz_msgs::GearReject_<ContainerAllocator> >::stream(s, indent + "  ", v.reject);
    s << indent << "override: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.override);
    s << indent << "fault_bus: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.fault_bus);
  }
};

} // namespace message_operations
} // namespace ros

#endif // DBW_MKZ_MSGS_MESSAGE_GEARREPORT_H
