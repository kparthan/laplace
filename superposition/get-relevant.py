import sys, codecs, os, re
if sys.stdout.encoding is None:
        sys.stdout = codecs.open("/dev/stdout", "w", 'utf-8')

fr = open('results_general_2','r')
fw = open('positive_results_general_2','w')
line = fr.readline();
line_count = 1
relevant = 0
while line != '':
  x = line.strip('\n')
  y = x.split()
  if len(y) > 8:
    temp = y[len(y)-8:]
    y = temp
    x = ''
    for i in range(8):
      x = x + '\t' + y[i]
  l2_msglen = float(y[5])
  l1_initial_msglen = float(y[6])
  l1_final_msglen = float(y[7])
  if l2_msglen >= l1_initial_msglen or l2_msglen >= l1_final_msglen:
    fw.write(x+'\n')
    relevant += 1
  line = fr.readline();
  print line_count
  line_count += 1

fw.close()
fr.close()

print "# of results: ", line_count
print "# of positive results: ", relevant

