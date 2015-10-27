# dataset begins 5 days previous to the day accessed
# starts at midnight, not sure what timezone
# 
#
#
# the timestamps generated are off by 1 hour
# guessing it's a difference between daylight savings time

import math
import xml.etree.ElementTree
import os
import urllib
import schedule
import time



def do_everything():

  opener = urllib.FancyURLopener({})
  f = opener.open("http://www.ieso.ca/Data/data/ontario_demand_multiday.xml")

  root = xml.etree.ElementTree.parse(f).getroot()

  #root = xml.etree.ElementTree.parse('ontario_demand_multiday-2.xml').getroot()

  # print(root.tag)
  # print("\n\n")
  # print(root.attrib)

  start_date = ""
  num_hours = 24
  num_mins = 60
  data_per_day = (24*60)/5
  data_per_set = data_per_day*6 # why do we have to multiply by 6? off by one somewhere
  print "total number of data points in this set = %d" % data_per_set
  print "num of data points in one day = %d" % data_per_day


  for child in root:
    if child.tag == "StartDate":
    	start_date = child.text

    # all the data in the data set
    if child.tag == "DataSet":
      d = child.attrib
      dataset_type = d["Series"]

      # all the data for 5 mins
      if dataset_type == "5_Minute":


        # --------------
        # extracting the value
        # --------------

        all_data = []

        for i in range(0, data_per_day+1):  # 0 - 288+1
          today_index = data_per_set-i      # 1728 - i (max 288)
          temp = child[today_index]         # 1728 (newest) - 1440 (oldest)
          demand_value = temp[0].text
          if demand_value != None:
            data_point = {}
            data_point['demand'] = demand_value
            data_point['time'] = ""
            all_data.append(data_point)

        

        # ---------------
        # generating the timestamps from the most current data point
        # ---------------

        all_data_len = len(all_data)
        print "len of all data %d" % all_data_len

        all_time_strs = []

        for i in range(0, all_data_len):

          total_mins = all_data_len*5
          total_hours = float(total_mins)/60

          # print "total num of mins: %d " % total_mins
          # print "total num of hours: %f " % math.floor(total_hours)

          the_mins = i*5
          the_hours = float(the_mins)/60

          time_hours = math.floor(the_hours)
          time_mins = the_mins-(math.floor(the_hours)*60)
          meridian = ""

          if time_hours == 12:
            meridian = "pm"
          elif time_hours > 12:
            time_hours = time_hours-12
            meridian = "pm"
          elif time_hours < 12:
            meridian = "am"

          time_str = ""
          if time_mins < 10:
            time_str = "%d:0%d %s" % (time_hours, time_mins, meridian)
          else:
            time_str = "%d:%d %s" % (time_hours, time_mins, meridian)

          all_time_strs.append(time_str)



        # -------------
        # updating the data dict with the timestamps
        # -------------

        for i in range(0, all_data_len):

          ind = ( all_data_len-1 ) - i
          update1 = {}
          update1['time'] = all_time_strs[i]
          all_data[ind].update(update1)



        # ---------------
        # printing it out - the reason why this has to be done
        # in a sepparate loop is because of the direction the
        # indices are going in the previous loop. there is a 
        # midpoint where there's a mismatch between what data
        # has and has not been added yet
        # ----------------

        for i in range(0, all_data_len):
          print "%d: %s" % (i, str(all_data[i]))
          


        # ----------------
        # reversing the data
        # ----------------

        result_data = []

        for i in range(0, all_data_len):
          meep = {}
          meep['demand'] = all_data[ all_data_len-1-i ]['demand']
          meep['time'] = all_data[ all_data_len-1-i ]['time']
          result_data.append(meep)
          



        # ---------------
        # export the dictionary to csv now
        # ---------------

        dir1 = os.path.dirname(os.path.realpath(__file__))
        fname = "5min_demand.csv"
        path1 = "%s/%s" % (dir1, fname)

        f = open(path1, 'w+')
        f.write("Time, Demand\n")

        for i in range(0, all_data_len):
          str1 = "%s, %s\n" % (all_data[i]['time'], all_data[i]['demand'])
          f.write(str1)

        f.close()


        
      elif dataset_type == "Actual":
        print "ok"


      elif dataset_type == "Projected":
        print "ok2"



  # print(start_date)




def job():
  print("working...")
  do_everything()


schedule.every(5).minutes.do(job)

while True:
  schedule.run_pending()
  time.sleep(1)


