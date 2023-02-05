import sys


#variables
clusters=[]
terminate=False
epsilon=0.0001


#helper functions
"""Given two vectors, this function calculates and returns 
    the distance between them (euclidean**2)"""
def distance(s, t):
    res=0.0
    for i in range(len(s)):
        res+=float((s[i]-t[i]))**2
    return res


"""Given two vectors, this function sums them into one vector, it sums
    each coordinate to the other in accordance and returns the summed vector"""
def sum(s, t):
    r=[]
    for i in range(len(s)):
        r.append(float(float(s[i])+float(t[i])))
    return r


"""Given an iterable object, this functions returns the size of it"""
def getSize(obj):
    cnt=0
    for elem in obj:
        cnt+=1
    return cnt


"""This functions returns a list of the centroids"""
def getCentroids():
    res=[]
    for cluster in clusters:
        res.append(cluster[0])
    return res


"""Given two clusters, this function calculates the delta of the euclidian norm between
    both clusters"""
def deltaNorm(oldClusters, newClusters):
    dists=[]
    for i in range(len(oldClusters)):
        res=distance(oldClusters[i][0], newClusters[i][0])
        dists.append(res**0.5)
    return dists


"""Given the distances between the old and the new centroids, this function updates
    the terminate condition if all the distances is less than epsilon"""
def readyToTerminate(distances):
    ready=True
    for dist in distances:
        if dist>=epsilon:
            ready=False
            break
    return ready


#functions
""""Given the k, this function makes k sublists in the clusters list"""
def makeClusters(k):
    for i in range(k):
        clusters.append([])


"""Given the index of the input file, this func reads the data and returns the quantity of point
    and a list of points"""
def readData(iFile_index):
    cnt=0
    points=[]
    f=open(sys.argv[iFile_index], "r")
    lines=f.readlines()
    for line in lines:
        cnt+=1
        s=[]
        for num in line.split(","):
            s.append(float(num))
        points.append(s)
    res=[cnt, points]
    f.close()
    return res


"""This func inits the first k points as the first k clusters"""
def initClusters(k, points):
    i=0
    while i<k:
        clusters[i].append(points[i])
        i+=1


""""This function assigns the points to the closest cluster
    the sign indicates whether it's the first iteration (sign==0)"""
def assignPoints(n, k, points, sign):
    if sign==0:
        i=k
    else:
        i=0
    while i<n:
        distances=[]
        for cluster in clusters:
            dist=distance(cluster[0], points[i])
            distances.append(dist)
        smallest=min(distances)
        clusterIndex=distances.index(smallest)
        clusters[clusterIndex].append(points[i])
        i+=1


"""Update the centroids and return new clusters"""
def centroidsUpdate():
    new=[]
    for j in range(len(clusters)):
        n=getSize(clusters[j])
        i=0
        r=clusters[j][i]
        while i<n-1:
            r=sum(r, clusters[j][i+1])
            i+=1
        for i in range(len(r)):
            r[i]=r[i]/n
        new.append([r])
    return new


"""Output function"""
def outFunc(k, clusters):
    for i in range(k):
        output = ''
        for j in range(len(clusters[i][0])):
            if(j==0):
                output = output + str("{:.4f}".format(float(clusters[i][0][j])))
                output=output +','
            else:
              if(j==(len(clusters[i][0])-1)):
                  output = output + str("{:.4f}".format(float(clusters[i][0][j])))
              else:
                  output = output + str("{:.4f}".format(float(clusters[i][0][j])))
                  output=output +','
        print(output)
        filetowrite = open(sys.argv[iFile_index+1], 'w')
        filetowrite.write(output)


argv = sys.argv
k = int(argv[1])
if(len(argv) == 5):
    maxIter = int(argv[2])
    iFile_index = 3
    outputFile = argv[4]
else:
    maxIter = 200
    iFile_index = 2
    outputFile = argv[3]


makeClusters(k)
dataArr=readData(iFile_index)
pointsCnt=dataArr[0]
points=dataArr[1]

if k>pointsCnt or k<0:
    print("Invalid Input!")
    exit(1)

initClusters(k, points)
x=0
while x<maxIter and not terminate:
    if x==0:
        assignPoints(pointsCnt, k, points, 0)
    else:
        assignPoints(pointsCnt, k, points, 1)
    oldClusters=clusters
    newClusters=centroidsUpdate()
    delta=deltaNorm(oldClusters, newClusters)
    terminate=readyToTerminate(delta)
    clusters=newClusters
    x+=1

outFunc(k, clusters)
