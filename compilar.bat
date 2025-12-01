@echo off
REM compilar extension neuronet usando entorno virtual para evitar restricciones del sistema
set VENV_DIR=.venv
set VENV_PY=%VENV_DIR%\Scripts\python.exe

if not exist %VENV_PY% (
	echo creando entorno virtual...
	python -m venv %VENV_DIR%
)

REM compatibilidad con entornos msys2: usar bin\python.exe si no existe Scripts\python.exe
if not exist %VENV_PY% (
	set VENV_PY=%VENV_DIR%\bin\python.exe
)

echo instalando dependencias necesarias (puede tardar)...
%VENV_PY% -m pip install --upgrade pip setuptools wheel
%VENV_PY% -m pip install -r requirements.txt

echo compilando extension neuronet...
%VENV_PY% setup.py build_ext --inplace
echo compilacion terminada.
pause
