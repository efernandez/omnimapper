#include <omnimapper/transform_tools.h>
#include <pcl/common/common.h>
#include <pcl/common/eigen.h>

gtsam::Pose3 transformToPose3(const Eigen::Affine3f& tform)
{
  //  return gtsam::Pose3(gtsam::Rot3(tform(0,0),tform(0,1),tform(0,2),
  //				  tform(1,0),tform(1,1),tform(1,2),
  //				  tform(2,0),tform(2,1),tform(2,2),
  //				  gtsam::Point3(tform());
  return gtsam::Pose3(tform.matrix().cast<double>());
}

Eigen::Affine3f pose3ToTransform(const gtsam::Pose3& pose)
{
  Eigen::Affine3f transform (pose.matrix ().cast<float>());
  return (transform);
  //gtsam::Vector ypr = pose.rotation().ypr();
  //Eigen::Affine3f t = pcl::getTransformation(pose.x(), pose.y(), pose.z(), ypr[2], ypr[1], ypr[0]);
  //return t;
}

Eigen::Affine3d planarAlignmentTransform (const Eigen::Vector4d& target, const Eigen::Vector4d& to_align)
{
  Eigen::Vector3d target_norm (target[0], target[1], target[2]);
  Eigen::Vector3d align_norm (to_align[0], to_align[1], to_align[2]);
  double angle = acos (align_norm.dot (target_norm));
  Eigen::Vector3d axis = align_norm.cross (target_norm);
  axis.normalize ();
  Eigen::Affine3d transform;
  // If the normal is near identical, we'll get an invalid transform, and should instead use identity
  if ((pcl_isfinite (angle)) && (pcl_isfinite (axis[0])) && (pcl_isfinite (axis[1])) && (pcl_isfinite (axis[2])))
  {
    transform = Eigen::AngleAxisd (angle, axis);
  }
  else
  {
    transform = Eigen::Affine3d::Identity ();
  }
  //transform = Eigen::AngleAxisd (angle, axis);
  transform.translation () = target_norm * -1 * (target[3] - to_align[3]);
  return (transform);
}

// void
// movePlaneToXY(const Eigen::Vector4d& input_coeffs, CloudPtr input_cloud, CloudPtr output_cloud)