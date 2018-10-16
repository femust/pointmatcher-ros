#pragma once

// std
#include <memory>

// std msgs
#include <std_msgs/Header.h>

// pointmatcher
#include <pointmatcher_ros/point_cloud.h>

// pcl
#include <pcl/io/ply_io.h>

// pcl conversions
#include <pcl_conversions/pcl_conversions.h>

// pointmatcher_ros
#include "pointmatcher_ros/PmTf.h"
#include "pointmatcher_ros/usings.h"

namespace PointMatcher_ros {

class StampedPointCloud {
 public:
  std_msgs::Header header_;
  PmDataPoints dataPoints_;

 protected:
  std::shared_ptr<PmTransformator> transformator_;

 public:
  StampedPointCloud();

  static StampedPointCloud FromFile(const std::string& filePath, const ros::Time& stamp, const std::string& frameId);
  bool fromFile(const std::string& filePath, const ros::Time& stamp, const std::string& frameId);
  void toFile(const std::string& filePath) const;

  static StampedPointCloud FromRosMsg(const sensor_msgs::PointCloud2& msg);
  void fromRosMsg(const sensor_msgs::PointCloud2& msg);
  sensor_msgs::PointCloud2 toRosMsg() const;
  void toRosMsg(sensor_msgs::PointCloud2& msg) const;

  StampedPointCloud createSimilarEmpty() const;

  bool isEmpty() const;
  unsigned int getSize() const;
  void clear();

  bool descriptorExists(const std::string& name) const;
  void addOneDimensionalDescriptor(const std::string& name, const float value);
  PmDataPointsView getDescriptorView(const std::string& name);
  PmDataPointsConstView getDescriptorConstView(const std::string& name) const;
  void setCurvatureFromStaticProbability();
  void setStaticProbabilityFromCurvature();

  bool transform(const PmTf& tf);

  bool filter(PmPointCloudFilters& filters);
  void filterByDistance(const float distanceThreshold, const bool keepInside);
  void filterByDistance(const float distanceThreshold, const bool keepInside, PmMatrix& newIdToOldId);
  void filterByStaticProbability(const float staticProbabilityThreshold, const bool keepStatic);

  bool add(const StampedPointCloud& other);
  bool addNonOverlappingPoints(const StampedPointCloud& other, const float maxDistOverlappingPoints);

  bool splitByOverlap(const StampedPointCloud& other, const float distanceThreshold, StampedPointCloud& otherOverlappingPoints,
                      StampedPointCloud& otherNonOverlappingPoints) const;
  void splitByStaticProbability(const float staticProbabilityThreshold, StampedPointCloud& staticPoints, StampedPointCloud& dynamicPoints) const;

  unsigned int countStaticPoints(const float staticProbabilityThreshold) const;

  PmMatrix toSphericalCoordinates() const;
};

std::ostream& operator<<(std::ostream& ostream, const StampedPointCloud& pointCloud);

}  // namespace PointMatcher_ros
