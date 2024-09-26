from setuptools import find_packages, setup
import os

package_name = 'perception'

launch_files = [
    'launch/perception.launch.py',
]

setup(
    name=package_name,
    version='0.0.1',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), launch_files),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='nick',
    maintainer_email='nickojbell@gmail.com',
    description='YOLO inference package using ROS2',
    license='MIT',
    tests_require=['pytest'],
    entry_points={
    'console_scripts': [
        'yolo_inference = perception.yolo_inference:main',
    ],
},

)
