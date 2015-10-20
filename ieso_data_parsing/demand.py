# dataset begins 5 days previous to the day accessed
# starts at midnight, not sure what timezone
# 

import xml.etree.ElementTree

root = xml.etree.ElementTree.parse('ontario_demand_multiday-2.xml').getroot()

# print(root.tag)
# print("\n\n")
# print(root.attrib)

global start_date

for child in root:
  if child.tag == "StartDate":
  	start_date = child.text

  # all the data in the data set
  if child.tag == "DataSet":
    d = child.attrib
    dataset_type = d["Series"]

    # all the data for 5 mins
    if dataset_type == "5_Minute":

      i = 0
      j = 0
      deedee = ""
      strstr = ""
      for datum in child:
        i = i+1
        for chichi in datum:
          if chichi.tag == "Value":
            if chichi.text != None:
              j = j+1
              strstr = chichi.text
        deedee = datum



      today_demands = []
      new_index = 0
      
      for i in range(0, 288):
        today_index = 1728-i
        temp = child[today_index]
        demand_value = temp[0].text
        print demand_value
        
        # if demand_value != None:
        #   today_demands[new_index] = demand_value
        #   new_index = new_index+1

        #print "demand value = %s" % demand_value

      # now let's double check the value
      # 1728-288=1440

      bleep = child[1728-288]
      bleepy = bleep[0]
      print "bleep: %s " % bleepy.text


      print "wat"

      print "counts: %d" % i
      print "last data index: %d" % j
      # print "last data: %s" % strstr

      # bloop = "a"

      # bloop = "a"
  	  
      # bloop = "a"

      # remaining = i-j
      # mins_today = remaining*5
      # hours_today = mins_today/60
      # current_hour = 24-hours_today
      # current_hour_12 = current_hour-12
      # am_pm = "am"
      # if current_hour >= 12:
      #   am_pm = "pm"

      # print "remaining data: %d" % remaining
      # print "mins_today: %d" % mins_today
      # print "hours_today: %d" % hours_today


      
    elif dataset_type == "Actual":
      print "ok"


    elif dataset_type == "Projected":
      print "ok2"



print(start_date)


num_hours = 24
num_mins = 60
num_5mins_day = (24*60)/5
times_5_days = num_5mins_day*6
print "total number of data points possible = %d" % times_5_days
print "num of data points in one day = %d" % num_5mins_day
# why do we have to multiply by 6? off by one somewhere


# for atype in e.findall('type'):
#   print(atype.get('foobar'))

