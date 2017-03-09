// Extract.cpp : Defines the entry point for the console application.
//
#include "util/testharness.h"
#include "leveldb/env.h"
#include "leveldb/table.h"
#include "util/logging.h"
#include "util/mutexlock.h"
#include "util/testharness.h"
#include "util/testutil.h"
#include "leveldb/db.h"
#include <iostream>
#include <fstream>
#include "caffe.pb.h"
using namespace std;
using namespace leveldb;
using namespace caffe;

int main(int argc, char* argv[])
{
    DB* db;
    Options options;
    options.create_if_missing = false;
    options.error_if_exists = false;
    string dbname;
    string outfile;
    if (argc > 1)
        dbname = argv[1];
    if (argc > 2)
        outfile = argv[2];
    else
        return -1;
    Status status = DB::Open(options, dbname, &db);
    //assert(status.ok());
    if (!status.ok()) {
        cerr << status.ToString() << endl;
        return -1;
    }
    caffe::Datum d;
    ofstream file(outfile); // , ios::binary);
    // iteration
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next())
    {
        //int k = atoi(it->key().ToString().c_str());
        string k = it->key().ToString();
        cout << k << endl;
        // parse protobuf
        d.clear_float_data();
        d.clear_data();
        d.ParseFromString(it->value().ToString());
        // output:
        file << k << "\t";
        for (int i = 0; i< d.height(); i++)
            file << "\t" << d.float_data(i);
        file << endl;
        // for binary:
        /*
        string data = it->value().ToString();
        int l = data.length();
        file.write((const char*)&k, sizeof(k));
        file.write((const char*)&l, sizeof(l));
        file.write(data.c_str(), l);
        */
    }
    /*if (!status.ok())
    {
    cerr << status.ToString() << endl;
    return -1;
    }*/
    // close
    file.close();
    delete db;
    return 0;
}

