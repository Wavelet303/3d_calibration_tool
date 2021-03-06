#ifndef __PLANE_FINDER__
#define __PLANE_FINDER__

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include <opencv2/opencv.hpp>

class ConfigDialog;

class PlaneFinder : public QThread
{
    Q_OBJECT

public:
    PlaneFinder(QObject* parent = 0);
    virtual ~PlaneFinder(void);

    void setConfigDialog(ConfigDialog* dialog);

public slots:
    void setInputCloud(pcl::PointCloud<pcl::PointXYZRGBL>::ConstPtr cloud);
    void generateCalibrationBoard(void);

signals:
    void foundPlane(pcl::PointCloud<pcl::PointXYZRGBL>::ConstPtr cloud, const pcl::PointXYZ& start,
                    const pcl::PointXYZ& end, const std::vector<cv::Point3f>& points);
    void foundAxis(const pcl::PointXYZ& start, const pcl::PointXYZ& end);

protected:
    virtual void run(void);

private:
    void search(void);
    void computeNormals(pcl::PointCloud<pcl::PointXYZRGBL>::ConstPtr cloud, pcl::PointCloud<pcl::Normal>::Ptr normals);
    void copyCloudToMat(pcl::PointCloud<pcl::PointXYZRGBL>::ConstPtr cloud, cv::Mat& mat);
    void computePoints(const Eigen::Vector3f& mean, const Eigen::Matrix3f& eigenvectors, std::vector<cv::Point3f>& points);

    QMutex _mutex;
    QWaitCondition _updated;
    pcl::PointCloud<pcl::PointXYZRGBL>::Ptr _inputCloud;
    pcl::PointCloud<pcl::PointXYZRGBL>::Ptr _planeCloud;
    pcl::PointCloud<pcl::PointXYZRGBL>::Ptr _caliBoardCloud;
    float _alpha;
    float _beta;
    float _gamma;
    cv::Mat _midPoint;
    ConfigDialog* _dialog;
};

#endif
