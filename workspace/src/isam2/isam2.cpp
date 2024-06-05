// #include "isam2.hpp"
#include <type_traits>

// Camera observations of landmarks will be stored as Point2 (x, y).
#include <gtsam/geometry/Point2.h>
#include <gtsam/geometry/Pose2.h>
#include <Eigen/Dense>
// Each variable in the system (sposes and landmarks) must be identified with a
// unique key. We can either use simple integer keys (1, 2, 3, ...) or symbols
// (X1, X2, L1). Here we will use Symbols
#include <gtsam/inference/Symbol.h>

// We want to use iSAM2 to solve the structure-from-motion problem
// incrementally, so include iSAM2 here
#include <gtsam/nonlinear/ISAM2.h>

// iSAM2 requires as input a set of new factors to be added stored in a factor
// graph, and initial guesses for any new variables used in the added factors
#include <gtsam/nonlinear/NonlinearFactorGraph.h>
#include <gtsam/nonlinear/Values.h>

// In GTSAM, measurement functions are represented as 'factors'. Several common
// factors have been provided with the library for solving robotics/SLAM/Bundle
// Adjustment problems. Here we will use Projection factors to model the
// camera's landmark observations. Also, we will initialize the robot at some
// location using a Prior factor.
#include <gtsam/slam/ProjectionFactor.h>
#include <gtsam/slam/BetweenFactor.h>
#include <gtsam/slam/BearingRangeFactor.h>

#include <vector>

#include <iostream>
#include <fstream>
using namespace std;
using namespace gtsam;

//static const float M_DIST_TH = 0.000151169; // for real data
static const float M_DIST_TH = 20;
// static const float M_DIST_TH = 45; // used to be 45 lmao
static const long SEC_TO_NANOSEC = 1000000000;

// static const float DT = 0.1;
// static const float SIM_TIME = 50.0;
// static const int LM_SIZE = 2;
// static const int STATE_SIZE = 3;
// static const int N_STEP = 100;

struct Landmark {
    int lm_id;
    gtsam::Pose2 lm_pos;
};

class Compare {
public:
    bool operator()(Landmark lm1, Landmark lm2) const {
        return (lm1.lm_pos.x() > lm2.lm_pos.x() && lm1.lm_pos.y() > lm2.lm_pos.y());
    }
};

class slamISAM {
private:
    ISAM2Params parameters;

    ISAM2 isam2;
    //Create a factor graph and values for new data
    NonlinearFactorGraph graph;
    Values values;

    int pose_num;


    gtsam::Symbol X(int robot_pose_id) {
        return Symbol('x', robot_pose_id);
    }

    gtsam::Symbol L(int cone_pose_id) {
        return Symbol('l', cone_pose_id);
    }

public:

    int n_landmarks; gtsam::Pose2 robot_est;
    std::vector<gtsam::Pose2> landmark_est;
    std::vector<Point2> orange_cones;

    slamISAM() {
        parameters = ISAM2Params(ISAM2DoglegParams(),0.1,10,true);
        parameters.setFactorization("QR");

        isam2 = gtsam::ISAM2(parameters);
        graph = gtsam::NonlinearFactorGraph();
        values = gtsam::Values();
        pose_num = 0;
        n_landmarks = 0;
        robot_est = gtsam::Pose2(0, 0, 0);
        landmark_est = std::vector<gtsam::Pose2>();

        orange_cones = std::vector<Point2>();
    }

    /**
     * @brief
     *
     * @param measurement: the global position of the observed cone at
     * current time step
     *
     * @param landmark; the isam2 estimate for global position of previously
     * seen cone. we want the covariance of this cone
     */
    double mahalanobisDist(auto logger, Pose2 measurement,Pose2 landmark,Symbol landmark_key){
        //mahalanobis distance with just x,y???
        Eigen::MatrixXd diff(1, 3);
        diff << measurement.x()-landmark.x(),measurement.y()-landmark.y(),1;
        Eigen::MatrixXd marginal_covariance = isam2.marginalCovariance(landmark_key);
        /*RCLCPP_INFO(logger, "\n%f | %f | %f\n %f | %f | %f\n %f | %f | %f\n ",
                    marginal_covariance(0, 0), marginal_covariance(0, 1), marginal_covariance(0, 2),
                    marginal_covariance(1, 0), marginal_covariance(1, 1), marginal_covariance(1, 2),
                    marginal_covariance(2, 0), marginal_covariance(2, 1), marginal_covariance(2, 2));*/

        //shouldn't this be inverse?

        Eigen::MatrixXd test = diff * marginal_covariance;
        RCLCPP_INFO(logger, "\n%f | %f | %f\n ",
                test(0, 0), test(0, 1), test(0, 2));

        Eigen::MatrixXd result = diff*marginal_covariance*diff.transpose();
        //size of eigen matrix is (1,1)
        return result(0);
    }





    //returns associated landmark id or n_landmarks if there is no associated id
    //returns zero on the first
    int associate(auto logger, Pose2 measurement) {
        // Vector that will store mahalanobis distances
        std::vector<double> min_dist;

        // Previous one
        //vectorize this
        //Eigen::VectorXd v_m_dist = v_associate(logger, measurement);

        RCLCPP_INFO(logger, "printing m_dist");
        for (int i = 0; i < n_landmarks; i++) {
            gtsam::Pose2 landmark = isam2.calculateEstimate(L(i)).cast<Pose2>();

            // Adding mahalanobis distance to minimum distance vector

            //TODO:make this into a matrix and do a single operation on this
            /*RCLCPP_INFO(logger, "actual diff %d: dx: %f \t dy: %f \t d0: %f", i,
                                                                     measurement.x() - landmark.x(),
                                                                     measurement.y() - landmark.y(),
                                                                     1.0);*/

            double mahalanobis = mahalanobisDist(logger, measurement,landmark,L(i));
            // RCLCPP_INFO(logger, "L(%d)=%f",i,mahalanobis);

            //this will already be calculated, so there's no reason to push back
            RCLCPP_INFO(logger, "p: (%f, %f) | \t m_dist: %f", landmark.x(), landmark.y(), mahalanobis);
            min_dist.push_back(mahalanobis); //i
            //assert(v_m_dist(i) == mahalanobis);
        }
        RCLCPP_INFO(logger, "min_id: %d | \t M_DIST_TH: %f", n_landmarks, M_DIST_TH);

        RCLCPP_INFO(logger, "m_dist print done");

        //TODO: you can add this beforehand
        min_dist.push_back(M_DIST_TH); // Add M_DIST_TH for new landmark
        // Find the index of the minimum element in 'min_dist'
        //min_id will be equal to num_landmarks if it didn't find anything under M_DIST_TH



        //TODO:find min
        int min_id = std::distance(min_dist.begin(), std::min_element(min_dist.begin(), min_dist.end()));
        //RCLCPP_INFO(logger, "Min_id: %d \t | \t n_lm: %d\n",min_id, n_landmarks);
        // RCLCPP_INFO(logger, "Min dist %f\n",min_dist[min_id]);

        return min_id;
    }

    /**
     * @brief print_cones will print the positions of the
     * observed cones stored within the vector cone_obs
     *
     * @param cone_obs is a memory address to the vector
     * containing the Point2 positions of the observed cones
     * - you can just pass in the variable, but the type is
     *   std::vector<Point2> &
     */
    void print_cones(auto logger, std::vector<Point2> &cone_obs)
    {
        RCLCPP_INFO(logger, "Printing cones");
        for (int i = 0; i < cone_obs.size(); i++)
        {
            RCLCPP_INFO(logger, "(%f, %f)\n", cone_obs.at(i).x(),
                                            cone_obs.at(i).y());
        }
        RCLCPP_INFO(logger, "done printing\n\n");
    }

    void print_matrix(auto logger, Eigen::MatrixXd m)
    {
        for (int r = 0; r < m.rows(); r++)
        {
            for (int c = 0; c < m.cols(); c++)
            {
                cout << m(r, c) << " | ";
            }
            cout << endl;
        }
    }

    void step(auto logger, gtsam::Pose2 global_odom, std::vector<Point2> &cone_obs, std::vector<Point2> &orange_ref_cones, gtsam::Point2 velocity,long time_ns, bool loopClosure) {

        //RCLCPP_INFO(logger, "stepping\n");


        Vector NoiseModel(3);
        NoiseModel(0) = 0;
        NoiseModel(1) = 0;
        NoiseModel(2) = 0;

        Vector LandmarkNoiseModel(3);
        //used to be 0.01 for real data
        //LandmarkNoiseModel(0) = 0.01;
        //LandmarkNoiseModel(1) = 0.01;
        //LandmarkNoiseModel(2) = 0.01;

        // No noise kinda good??
        LandmarkNoiseModel(0) = 0.0;
        LandmarkNoiseModel(1) = 0.0;
        LandmarkNoiseModel(2) = 0.0;
        //print_cones(logger, cone_obs);
        static auto landmark_model = noiseModel::Diagonal::Sigmas(LandmarkNoiseModel);

        if (pose_num==0) {//if this is the first pose, add your inital pose to the factor graph
            //std::cout << "First pose\n" << std::endl;
            static noiseModel::Diagonal::shared_ptr prior_model = noiseModel::Diagonal::Sigmas(NoiseModel);
            gtsam::PriorFactor<Pose2> prior_factor = gtsam::PriorFactor<Pose2>(X(0), global_odom, prior_model);
            //add prior
            graph.add(prior_factor);
            values.insert(X(0), global_odom);


            //ASSUMES THAT YOU SEE ORANGE CONES ON YOUR FIRST MEASUREMENT OF LANDMARKS
            //Add orange cone left and right
            //hopefully it's only 2 cones
            orange_cones = orange_ref_cones;
        }
        else {
            //std::cout << "New Pose\n" << std::endl;
            static noiseModel::Diagonal::shared_ptr odom_model = noiseModel::Diagonal::Sigmas(NoiseModel);
            Pose2 prev_pos = isam2.calculateEstimate(X(pose_num - 1)).cast<Pose2>();
            //create a factor between current and previous robot pose
            //add odometry estimates
            //Motion model


            //TODO: change back to motion model with velocity
            double time_s = time_ns/SEC_TO_NANOSEC;

            Pose2 Odometry =  Pose2(velocity.x()*time_s, velocity.y()*time_s, global_odom.theta() - prev_pos.theta());
            /*real data motion model?
            Pose2 Odometry =  Pose2(global_odom.x() - prev_pos.x(),global_odom.y() - prev_pos.y(),
                                    global_odom.theta() - prev_pos.theta());
                                    */

            static noiseModel::Diagonal::shared_ptr prior_model = noiseModel::Diagonal::Sigmas(NoiseModel);
            gtsam::PriorFactor<Pose2> prior_factor = gtsam::PriorFactor<Pose2>(X(0), global_odom, prior_model);
            //add prior
            graph.add(prior_factor);

            gtsam::BetweenFactor<Pose2> odom_factor = gtsam::BetweenFactor<Pose2>(X(pose_num - 1), X(pose_num),Odometry, odom_model);
            graph.add(odom_factor);
            values.insert(X(pose_num), global_odom);
        }

        if(loopClosure){
            // std::cout<<"loop closure constraint added"<<std::endl;
            static noiseModel::Diagonal::shared_ptr loop_closure_model = noiseModel::Diagonal::Sigmas(NoiseModel);
            //left is 0, right is 1
            //Do triangulation given the current cone positions and the previous saved orange cones
            //take the orange cones, look at difference between
            //the left and right cones
            Point2 diffLeft = orange_cones[0] - orange_ref_cones[0];
            Point2 diffRight = orange_cones[1] - orange_ref_cones[1];
            Point2 avgDiff = (diffLeft+diffRight)/2; //pose difference between first pose and last pose
            double angle = atan2(avgDiff.y(), avgDiff.x());
            Pose2 AvgPoseDiff = Pose2(avgDiff.x(), avgDiff.y(),angle);
            // std::cout<<"loop closure pose diff:"<<AvgPoseDiff<<std::endl;

            gtsam::BetweenFactor<Pose2> odom_factor = gtsam::BetweenFactor<Pose2>(X(0), X(pose_num),AvgPoseDiff, loop_closure_model);
            graph.add(odom_factor);
            values.insert(X(pose_num), global_odom);
        }

        //todo only do this once after update
        isam2.update(graph, values);
        graph.resize(0);
        values.clear();
        // std::cout << "global_odom: "  << global_odom << std::endl;

        RCLCPP_INFO(logger, "DATA ASSOCIATION BEGIN: Pose: (%f, %f)", global_odom.x(),
                                                                        global_odom.y());
        // DATA ASSOCIATION BEGIN


////////////////////////////////////////////////////////////////
        //Vectorized implementation

        //Calculate the positions of all cones current
        landmark_est.clear();
        for (int i = 0; i < n_landmarks; i++) {
            gtsam::Pose2 landmark = isam2.calculateEstimate().at(L(i)).cast<Pose2>();
            landmark_est.push_back(landmark);
            //TODO:maybe make this into a matrix as well
        }

        //Make the incoming cones into an eigen matrix
        Eigen::MatrixXd cone_meas(cone_obs.size(),2);
        //TODO: make sure this actually works
        for (int i = 0; i < cone_obs.size(); i++) {
            cone_meas(i) = cone_obs.at(i).x(),cone_obs.at(i).y();
        }

        //Range
        Eigen::MatrixXd range(cone_obs.size(),1);
        //range = cone_meas.rowwise().norm();
        for (int i = 0; i < cone_obs.size(); i++)
        {
            range(i) = norm2(cone_obs.at(i));
        }

        //Angle: eigen atan2 is sus
        //TODO: find a faster way to do this
        Eigen::MatrixXd bearing(cone_obs.size(),1);
        for (int i = 0; i < cone_obs.size(); i++) {
            bearing(i) = atan2(cone_obs.at(i).y(),cone_obs.at(i).x());
        }

        Eigen::MatrixXd totalBearing = bearing.array()+global_odom.theta();
        Eigen::MatrixXd global_cone_x(cone_obs.size(),1);
        Eigen::MatrixXd global_cone_y(cone_obs.size(),1);

        Eigen::MatrixXd totalBearing_cos(cone_obs.size(),1);
        Eigen::MatrixXd totalBearing_sin(cone_obs.size(),1);
        totalBearing_cos = totalBearing.array().cos();
        totalBearing_sin = totalBearing.array().sin();


        global_cone_x = global_odom.x() + range.array()*totalBearing_cos.array();
        global_cone_y = global_odom.y() + range.array()*totalBearing_sin.array();



        RCLCPP_INFO(logger, "Printing global obs: (n_landmarks: %d)", n_landmarks);
        /* iterate through observed cones: given as relative distance wrt car */
        for (int i = 0; i < cone_obs.size(); i++) {

            Pose2 conePose(cone_obs.at(i).x(),cone_obs.at(i).y(),0);
            //double range = norm2(cone);

            //double bearing = std::atan2(conePose.y(), conePose.x());//+ global_odom.theta();
            //double global_cone_x = global_odom.x() + range*cos(bearing+global_odom.theta());
            //double global_cone_y = global_odom.y() + range*sin(bearing+global_odom.theta());

            /* calculate global position of the cone */
            Pose2 global_cone(global_cone_x(i),global_cone_y(i),0);

            /* vectorize associate()
             *
             * 1.) get the isam2 estimate for previously seen landmark
             * 2.) calculate the difference in global position
             * - bearing set to 1
             * 3.) in the meantime, stack up marginal covariance matrices
             *
             * */

            /* each row represents difference between global position of
             * observed cone and previous cone estimate
             * -there are n_landmarks previous cone estimates
             *
             * the difference is a 3 element vector, which is why the columns
             * must be 3 * n_landmarks
             */
            /*
            */
            RCLCPP_INFO(logger, "associating\n");

            //int associated_ID = associate(logger, global_cone);
            int associated_ID = associate(logger, global_cone);

            //RCLCPP_INFO(logger, "cur obs: (%f, %f) | min_id: %d", global_cone_x, global_cone_y, associated_ID);
            //If it is a new cone:
            if (associated_ID == n_landmarks) { //if you can't find it in the list of landmarks
                //add cone to list
                //add factor between pose and landmark
                graph.add(BetweenFactor<Pose2>(X(pose_num), L(associated_ID),
                                                Pose2(conePose.x(), conePose.y(), bearing(i)),
                                                        landmark_model));
                //this is how we model noise for the environmant
                RCLCPP_INFO(logger, "new landmark\n");
                values.insert(L(n_landmarks), global_cone);

                if (n_landmarks == 0) {
                    graph.addPrior(L(0),conePose); //TODO: how does this prior make sense?
                }

                n_landmarks++;
            } else {
                // RCLCPP_INFO(logger, "Associated Landmark: %d", associated_ID);
                // std::cout << "Associated Landmark:\n"  << L(n_landmarks) << std::endl;
                //Add a factor to the associated landmark
                graph.add(BetweenFactor<Pose2>(X(pose_num), L(associated_ID), Pose2(conePose.x(),
                                                    conePose.y(), bearing(i)), landmark_model));
            }
            // RCLCPP_INFO(logger, "updating");

            isam2.update(graph, values);
            graph.resize(0);
            values.clear();
            Pose2 est = isam2.calculateEstimate(L(associated_ID)).cast<Pose2>();
            //RCLCPP_INFO(logger,"isam2 est: (%f, %f)\n", est.x(), est.y());

        }

        RCLCPP_INFO(logger, "DATA ASSOCIATION END\n");

        // DATA ASSOCIATION END

        //Print to squirrel.txt
        //Every single time you run step, you reprint the isam2 estimates
        //Remove the previous estimates (trunc)
        //RCLCPP_INFO(logger, "graphing\n");
        std::ofstream ofs;
        std::ofstream out("squirrel.txt");
        std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
        std::cout.rdbuf(out.rdbuf());
        ofs.open("squirrel.txt", std::ofstream::out | std::ofstream::trunc);
        auto estimate = isam2.calculateEstimate();
        estimate.print("Estimate:");
        ofs.close();
        std::cout.rdbuf(coutbuf); //reset to standard output again


        //calculate estimate of robot state
        robot_est = isam2.calculateEstimate().at(X(pose_num)).cast<gtsam::Pose2>();
        pose_num++;
        // std::cout << "Robot Estimate:(" << robot_est.x() <<"," << robot_est.y() << ")" << std::endl;
    }
};
