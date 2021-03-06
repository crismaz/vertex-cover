import os, sys
from subprocess import *
from resource import *

def is_exe(f):
    ''' Check if the given path is a valid executable file '''
    return os.path.isfile(f) and os.access(f, os.X_OK)

def read_range(expected_type):
    '''
    Read from stdin a range of values
    
    Input can be a single value, then a list containing only this value is returned.
    Otherwise, the input should contain two values (start and end) and an int (n).
    Then the resulting list contains n equally spaced values, first equal to start,
    and last equal to end.
    
    Args:
        expected_type: a function to convert from string to considered values, int or float
        
    Returns:
        a list containing all values in the given range
    '''
    
    inp = raw_input().split(' ')
    
    if len(inp) == 1:
        return [expected_type(inp[0])]
    
    start, end = map(expected_type, inp[:2])
    num_of_values = int(inp[2])
    
    assert num_of_values >= 2

    interval_length = end - start
    assert interval_length > 0

    steps = num_of_values - 1
    step = expected_type(interval_length / steps)
    
    curr = start
    result = []

    for i in range(num_of_values):
        result.append(curr)
        
        ''' Compensate for int division rounding '''
        if expected_type == int and i == num_of_values - 1 - interval_length % steps:
            step += 1
        
        curr += step
    
    return result

def get_time():
    ''' Return user time since the beginning of execution '''
    return getrusage(RUSAGE_CHILDREN).ru_utime
    
def run_with_input(path, args, inp):    
    proc = Popen(["./" + path] + args, bufsize=0, stdin=PIPE, stdout=PIPE, stderr=STDOUT)
    return proc.communicate(input = inp)[0].decode()
    
raw = False
    
def proper_print(s):
    if not raw: print s

def main():
    if len(sys.argv) < 3:
        print "Usage: python test.py VERTEX_COVER_BIN_DIR TEST_GENERATOR_BIN_DIR [-raw]"
        return 
        
    vc_bin = sys.argv[1]
    test_gen_bin = sys.argv[2]
    
    if not is_exe(vc_bin):
        print "Invalid vertex cover solver binary"
        return
        
    if not is_exe(test_gen_bin):
        print "Invalid test generator binary"
        return
    
    if len(sys.argv) > 3 and sys.argv[3] == "-raw":
        global raw
        raw = True
    
    proper_print ("Enter:\n"
                  "\t- number of vertices (int)\n"
                  "\t- ratio of vertex cover size to the number of vertices (float)\n"
                  "\t- density (float)")
           
    proper_print ("For each, enter either a single value, or a range in the format:"
                  "START END NUM_OF_VALUES")

    proper_print ("\nNumber of vertices:")
    sizes = read_range(int)
    
    proper_print ("\nVertex cover ratio:")
    vertex_cover_ratios = read_range(float)
    
    proper_print ("\nDensity:")
    densities = read_range(float)
    
    tmp = ""
    while tmp != "T" and tmp != "F":
        proper_print ("\nUse linear programming? (T/F)")
        tmp = raw_input()
    
    useLinearProgramming = (tmp == "T")
    
    if useLinearProgramming:
        tmp = ""
        while tmp != "T" and tmp != "F":
            proper_print ("\nUse any lp solution(if true, the program will not check if"
                          "all-1/2 is the only solution)? (T/F)")
            tmp = raw_input()
        
        anyLpSolution = (tmp == "T")
    else:
        anyLpSolution = False
        
    args = []

    if useLinearProgramming:
        args.append("-useLp")
    
    if anyLpSolution:
        args.append("-anyLpSolution")
    
    for vc_ratio in vertex_cover_ratios:
        for density in densities:
            proper_print ("\nvc_ratio = %.2f, density = %.2f" % (vc_ratio, density))
        
            for size in sizes:
                vc_size = int(size * vc_ratio)
                inp = "%s %s %s" % (size, vc_size, density)
                
                ''' Run test generator and save output as out '''
                out = run_with_input(test_gen_bin, [], inp)
                
                start_time = get_time()
                
                ''' Run vertex cover solver '''
                run_with_input(vc_bin, args, out)
                
                time_taken = get_time() - start_time
                
                if raw:
                    print "%d %.2f" % (size, time_taken)
                else:
                    print str(size) + ' ' * (6 - len(str(size))) + "%6.2fs" % time_taken

                
if __name__=="__main__":
    main()

