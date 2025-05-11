# Use an official Python runtime as a parent image
FROM python:3.8-slim-buster

# Set the working directory in the container to /app
WORKDIR /app

# Add the current directory contents into the container at /app
ADD . /app

# Install any needed packages specified in requirements.txt
RUN pip install --no-cache-dir -r requirements.txt

# Install Streamlit 
RUN pip install --no-cache-dir streamlit

# Expose port for Streamlit
EXPOSE 8501

# Run your_program.py when the container launches
CMD streamlit run app.py