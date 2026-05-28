from pathlib import Path
import subprocess

#enter here your user in linux, so the script will be able to 
# change the permissions of the test files to execute them.
USER = "liad"

def run_tests(test_path_dir="tests"):
    test_path = Path(test_path_dir)
    subprocess.run(f"sudo chown {USER}:{USER} chmod -R {test_path_dir}",shell=True)
    subprocess.run(["find", test_path_dir, "-type", "f", "-name", "*.sh", "-exec", "chmod", "+x", "{}", "+"])
    if not test_path.exists():
        print(f"Error: Directory {test_path_dir} does not exist.")
        return

    for test_dir in test_path.iterdir():
        if test_dir.is_dir():
            run_test(test_dir)



GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
RESET = "\033[0m" 

def run_test(test_dir):
    test_dir = Path(test_dir)
    
    test_file = test_dir / f"{test_dir.name}.sh"
    expected_output_file = test_dir / f"{test_dir.name}.expected"
    res_file = test_dir / f"{test_dir.name}.result"

    expected_output = ""

    if expected_output_file.exists():
        expected_output = expected_output_file.read_text()

    if test_file.exists():
        result = subprocess.run(
            [str(test_file)], 
            capture_output=False,  # אנחנו לא משתמשים ב-capture_output כי נגדיר ידנית
            stdout=subprocess.PIPE, 
            stderr=subprocess.STDOUT,  # מזרים את השגיאות לתוך ה-stdout
            text=True, 
            check=False
        )        
        if expected_output == result.stdout:
            print(f"{GREEN}[✓] Test {test_dir.name} passed{RESET}")
            if res_file.exists():
                res_file.unlink()
        else:
            res_file.write_text(result.stdout)
            print(f"{RED}[✗] Test {test_dir.name} failed. See {res_file} for details{RESET}")
    else:
        print(f"{YELLOW}[!] Skipping {test_dir.name}: Script file {test_file.name} not found.{RESET}")


if __name__ == "__main__":
    run_tests()
    
