import sys
sys.path.append("/home/shun/software/python/")

from lib import *

theta=.3
h=ROOT.TH2D("h",";;",100,-1,1,100,-1,1)
for i in range(int(1e4)):
    x=random.gauss(0.1,.03)
    y=random.gauss(-.2,.3)
    x2=np.cos(theta)*x+np.sin(theta)*y
    y2=-np.sin(theta)*x+np.cos(theta)*y
    h.Fill(x2,y2)
c=ROOT.TCanvas("c")
c.Draw()
h.Draw("colz")
c.SaveAs("RootTest.pdf")


