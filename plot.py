import matplotlib.pyplot as plt
import sys
from matplotlib.ticker import FormatStrFormatter

def main():
    for name in sys.argv[1:]:
        f = open(name, 'r')
        
        x = []
        y = []
        
        while True:
            line = f.readline()
            
            if line == "":
                break
                
            line = line.rstrip().split(' ')
            
            x.append(int(line[0]))
            y.append(float(line[1]))

        plt.plot(x, y, label = name)
    
    plt.xlabel("Number of vertices")
    plt.ylabel("Time taken")
    
    plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%d s'))
    plt.legend()
    
    plt.show()

if __name__=="__main__":
    main()
