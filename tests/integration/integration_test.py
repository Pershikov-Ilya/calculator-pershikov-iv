import subprocess

APP = "./build/app.exe"

def test_valid_sum_int():
    res = subprocess.run([APP], input="2+3\n", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout.strip()) == 5

def test_valid_sub_mult_int():
    res = subprocess.run([APP], input="5-3*2\n", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout.strip()) == -1

def test_valid_div_int():
    res = subprocess.run([APP], input="10/3\n", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout.strip()) == 3

def test_valid_brackets_int():
    res = subprocess.run([APP], input="(4+5)*6\n", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout.strip()) == 54

def test_valid_complex_int():
    res = subprocess.run([APP], input="8/2*(2+2)\n", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout.strip()) == 16

def test_valid_div_int_7_2():
    res = subprocess.run([APP], input="7/2\n", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout.strip()) == 3

def test_valid_long_expression_int():
    res = subprocess.run([APP], input="1+2+3+4\n", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout.strip()) == 10

def test_valid_sum_float():
    res = subprocess.run([APP, "--float"], input="3+2\n", text=True, capture_output=True)
    assert res.returncode == 0
    assert round(float(res.stdout.strip()), 4) == 5.0000

def test_valid_div_float():
    res = subprocess.run([APP, "--float"], input="7/2\n", text=True, capture_output=True)
    assert res.returncode == 0
    assert round(float(res.stdout.strip()), 4) == 3.5000

def test_valid_nested_brackets_int():
    res = subprocess.run([APP], input="((2+3)*(4-1))\n", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout.strip()) == 15

def test_error_invalid_char():
    res = subprocess.run([APP], input="2+a\n", text=True, capture_output=True)
    assert res.returncode == 1

def test_error_div_zero_int():
    res = subprocess.run([APP], input="5/0\n", text=True, capture_output=True)
    assert res.returncode == 1

def test_error_div_zero_float():
    res = subprocess.run([APP, "--float"], input="5/0\n", text=True, capture_output=True)
    assert res.returncode == 1

def test_error_unmatched_bracket_1():
    res = subprocess.run([APP], input="(2+3\n", text=True, capture_output=True)
    assert res.returncode == 1

def test_error_multiple_operators():
    res = subprocess.run([APP], input="1++2\n", text=True, capture_output=True)
    assert res.returncode == 1

def test_error_invalid_string():
    res = subprocess.run([APP], input="2+3abc\n", text=True, capture_output=True)
    assert res.returncode == 1

def test_whitespace_handling():
    res = subprocess.run([APP], input=" 2   +  (  3  * 4 )  \n", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout.strip()) == 14

def test_operator_precedence():
    res = subprocess.run([APP], input="2+3*4\n", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout.strip()) == 14

def test_operator_precedence_brackets():
    res = subprocess.run([APP], input="(2+3)*4\n", text=True, capture_output=True)
    assert res.returncode == 0
    assert int(res.stdout.strip()) == 20

def test_excessively_long_input():
    long_input = "1" * 1100 + "\n"
    res = subprocess.run([APP], input=long_input, text=True, capture_output=True)
    # Проверяем, что приложение завершилось с ошибкой (код 1)
    assert res.returncode == 1
