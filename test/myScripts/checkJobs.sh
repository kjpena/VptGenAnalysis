with open('/afs/cern.ch/user/test.txt','r')  as f:
    line_words = [line.split() for line in f]
    
    for i in xrange(0,121):
        x=line_words[i]
        wgtList.append( (int(x[0]),
                         float(x[3].split('=')[1]),
                         float(x[4].split('=')[1])) )
