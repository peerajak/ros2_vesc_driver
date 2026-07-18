#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/float64.hpp>

using std::placeholders::_1;

class VescDiffDrive : public rclcpp::Node
{
public:
  VescDiffDrive()
  : rclcpp::Node("vesc_diff_drive")
  {
    // ประกาศพารามิเตอร์
    this->declare_parameter<double>("wheel_base", 0.3);
    this->declare_parameter<double>("max_rpm", 1000.0);
    this->declare_parameter<double>("rpm_per_mps", 500.0);

    wheel_base_ = this->get_parameter("wheel_base").as_double();
    max_rpm_ = this->get_parameter("max_rpm").as_double();
    rpm_per_mps_ = this->get_parameter("rpm_per_mps").as_double();

    // Publishers
    master_pub_ = this->create_publisher<std_msgs::msg::Float64>(
      "/commands/motor_master/speed", 10);
    slave_pub_ = this->create_publisher<std_msgs::msg::Float64>(
      "/commands/motor_slave/speed", 10);

    // Subscriber
    cmd_vel_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "/cmd_vel", 10, std::bind(&VescDiffDrive::cmdVelCallback, this, _1));

    RCLCPP_INFO(this->get_logger(), "VESC Diff Drive Controller Started");
    RCLCPP_INFO(this->get_logger(), "wheel_base: %.2f, max_rpm: %.2f, rpm_per_mps: %.2f",
                wheel_base_, max_rpm_, rpm_per_mps_);
  }

private:
  void cmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
  {
    double linear = msg->linear.x;
    double angular = msg->angular.z;

    double left_rpm = linear - (angular * wheel_base_ / 2.0);
    double right_rpm = linear + (angular * wheel_base_ / 2.0);

    left_rpm *= rpm_per_mps_;
    right_rpm *= rpm_per_mps_;

    left_rpm = std::max(-max_rpm_, std::min(max_rpm_, left_rpm));
    right_rpm = std::max(-max_rpm_, std::min(max_rpm_, right_rpm));

    auto master_msg = std_msgs::msg::Float64();
    auto slave_msg = std_msgs::msg::Float64();
    master_msg.data = left_rpm;
    slave_msg.data = right_rpm;

    master_pub_->publish(master_msg);
    slave_pub_->publish(slave_msg);

    RCLCPP_DEBUG(this->get_logger(), "Left: %.2f, Right: %.2f", left_rpm, right_rpm);
  }

  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_sub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr master_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr slave_pub_;

  double wheel_base_;
  double max_rpm_;
  double rpm_per_mps_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<VescDiffDrive>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}