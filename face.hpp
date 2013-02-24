#ifndef FACE_HPP
#define FACE_HPP

#include <okapi/config.hpp>
#include <okapi/types/basictypes.hpp>
#include "PhMdetector.hpp"
#include "GRNN.hpp"

#include <string>
#include <vector>
#include <iostream>

#include <okapi.hpp>
#include <cv.h>
#include <math.h>


class FaceStore;
class Estimator;
class Face;
class Face {
    public:
        Face(){
            width = 0;
            height = 0;
            intered = false;
            pose = 0.0;
        }
        int frameIndex;
        cv::Point center;
        int width;
        int height;
        bool intered;
        float pose;

        bool near(Face compare){
            double curD = sqrt(pow((center.x-compare.center.x),2) + pow((center.y - compare.center.y),2));
            if(curD < 20)
                return true;
            else
                return false;
        }
        cv::Rect toRect(){
            cv::Rect curL;
            curL.x = center.x - width/2;
            curL.y = center.y - height/2;
            curL.width = width;
            curL.height = height;
            return curL;
        }
        void print(){
            cout<<"Face debug ---- "<<endl;
            cout<<"Face x:"<<center.x<<endl;
            cout<<"Face y:"<<center.y<<endl;
            cout<<"Face width:"<<width<<endl;
            cout<<"Face height:"<<height<<endl;
        }
};


class FaceStore
{
    public:
        FaceStore(); // default constructor, create a storage only
        ~FaceStore();
        void add(int x, int y, int width, int height, int frameIndex);
        vector<vector<Face> > cluster(); // return a clustered vector
        vector<vector<Face> > getClustered() const;
        vector<Face> getClustered(int i) const;
        vector<Face> getFacesAtFrame(int i) const;
        void interpolate();
        void interpolate(int clusterIndex);
        void printOut();
        vector<vector<int> > getVotes() const;
        void sort();
        void generatePoses(PhM::pose_estimator& my_est, cv::Mat& gray, int frameIndex);
        

    private:
        /* private function */
        vector<Face> fillIn(vector<Face> input);
        void reduceNoise(int tolerance); // if within tolerance no next, get rid of the frame 
        bool near(Face data, vector<Face> cluster);
        vector< vector<Face> > quickSort(vector< vector<Face> > store); // sort the clustered faces accordding to their y positions


        /* private data member */
        vector<Face> store;
        vector< vector<Face> > clustered;
        int curIndex;
        int totalFrame;
        vector< vector<int> > votes;
};

#endif
