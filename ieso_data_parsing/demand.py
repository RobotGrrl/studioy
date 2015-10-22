# dataset begins 5 days previous to the day accessed
# starts at midnight, not sure what timezone
# 

import math
import xml.etree.ElementTree

root = xml.etree.ElementTree.parse('ontario_demand_multiday-2.xml').getroot()

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


      # ----------
      # extracting the value
      # ----------

      all_data = []
      today_points = []
      new_index = 0

      for i in range(0, data_per_day+1):  # 0 - 288+1
        today_index = data_per_set-i      # 1728 - i (max 288)
        temp = child[today_index]         # 1728 (newest) - 1440 (oldest)
        demand_value = temp[0].text
        if demand_value != None:
          today_points.append(demand_value)

          data_point = {}
          data_point['demand'] = demand_value
          print data_point['demand']
          all_data.append(data_point)


      print "today_points = %d" % len(today_points)


      print all_data


      # ----------
      # determining the time of the most current data point
      # ----------

      for i in range(0, len(today_points)):

        total_mins = len(today_points)*5
        total_hours = float(total_mins)/60

        # print "total num of mins: %d " % total_mins
        # print "total num of hours: %f " % math.floor(total_hours)

        the_mins = i*5
        the_hours = float(the_mins)/60

        time_hours = math.floor(the_hours)
        time_mins = the_mins-(math.floor(the_hours)*60)
        meridian = ""

        # previous version
        # time_hours = math.floor(total_hours)
        # time_mins = total_mins-(math.floor(total_hours)*60)
        # meridian = ""

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

        # is there a better way to do this?
        meep = all_data[(len(today_points)-1)-i].copy()
        meep['time'] = time_str
        all_data[i] = meep

        #print time_str



      # -------
      # reversing it...
      # -------

      result_data = []

      print len(all_data)

      for i in range(0, len(all_data)):
        result_data.append( all_data[ len(all_data)-1-i ] )


      print result_data[1]
      print all_data[ len(all_data)-2 ]


      # -----------
      # export the dictionary to csv now
      # -----------

      #for item in all_data:



      print all_data[0]
      print all_data[1]
      print all_data[2]

      
    elif dataset_type == "Actual":
      print "ok"


    elif dataset_type == "Projected":
      print "ok2"



print(start_date)






# for atype in e.findall('type'):
#   print(atype.get('foobar'))

