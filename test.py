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
    
def run_with_input(path, inp):
    proc = Popen(["./" + path], bufsize=0, stdin=PIPE, stdout=PIPE, stderr=STDOUT)
    return proc.communicate(input = inp)[0].decode()

def main():
    if len(sys.argv) != 3:
        print "Usage: python test.py VERTEX_COVER_BIN_DIR TEST_GENERATOR_BIN_DIR"
        return 
        
    vc_bin = sys.argv[1]
    test_gen_bin = sys.argv[2]
    
    if not is_exe(vc_bin):
        print "Invalid vertex cover solver binary"
        return
        
    if not is_exe(test_gen_bin):
        print "Invalid test generator binary"
        return
        
    print ("Enter:\n"
           "\t- number of vertices (int)\n"
           "\t- ratio of vertex cover size to the number of vertices (float)\n"
           "\t- density (float)")
           
    print ("For each, enter either a single value, or a range in the format:"
           "START END NUM_OF_VALUES")

    print "\nNumber of vertices:"
    sizes = read_range(int)
    
    print "\nVertex cover ratio:"
    vertex_cover_ratios = read_range(float)
    
    print "\nDensity:"
    densities = read_range(float)
    
    for size in sizes:
        for vc_ratio in vertex_cover_ratios:
            vc_size = int(size * vc_ratio)
            
            print "\nn = %d, vc_size = %d" % (size, vc_size)
        
            for density in densities:
                print "density = %.2f, time:" % density,
                inp = "%s %s %s" % (size, vc_size, density)
                
                ''' Run test generator and save output as out '''
                out = run_with_input(test_gen_bin, inp)
                
                start_time = get_time()
                
                ''' Run vertex cover solver '''
                run_with_input(vc_bin, out)
                                
                print "%6.2fs" % (get_time() - start_time)

                
if __name__=="__main__":
    main()

