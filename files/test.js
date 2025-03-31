const fs = require('fs');
const path = require('path');

// 配置项
const config = {
    directoryPath: __dirname,
    selectedFolderName: '精选',
    imageExtensions: ['.jpg', '.jpeg'],
    rawFileExtension: '.CR2'
};

// 定义目标目录
const selectedFolderPath = path.join(config.directoryPath, config.selectedFolderName);

// 移动的文件计数器
let movedFilesCount = 0;

// 扫描目录中的文件
fs.readdir(config.directoryPath, (err, files) => {
    if (err) {
        return console.error('无法扫描目录: ' + err.message);
    }

    // 过滤出图像文件
    const imageFiles = files.filter(file => {
        return config.imageExtensions.includes(path.extname(file).toLowerCase());
    });

    if (imageFiles.length === 0) {
        console.log('没有找到需要移动的文件。');
        return;
    }

    // 创建精选文件夹（如果不存在）
    createFolderIfNotExists(selectedFolderPath);

    // 移动图像文件及其同名的原始文件到精选文件夹
    imageFiles.forEach(file => {
        const baseName = path.basename(file, path.extname(file));
        const rawFile = `${baseName}${config.rawFileExtension}`;

        const oldImagePath = path.join(config.directoryPath, file);
        const newImagePath = path.join(selectedFolderPath, file);

        // 移动图像文件
        moveFile(oldImagePath, newImagePath, file);

        // 检查并移动同名的原始文件
        const oldRawPath = path.join(config.directoryPath, rawFile);
        const newRawPath = path.join(selectedFolderPath, rawFile);

        if (fs.existsSync(oldRawPath)) {
            moveFile(oldRawPath, newRawPath, rawFile);
        } else {
            console.warn(`没有找到对应的原始文件: ${rawFile}`);
        }
    });

    console.log(`总共移动了 ${movedFilesCount} 个文件。`);

    // 删除该脚本自身
    deleteScriptSelf();
});

function createFolderIfNotExists(folderPath) {
    if (!fs.existsSync(folderPath)) {
        try {
            fs.mkdirSync(folderPath);
        } catch (err) {
            console.error('无法创建目标目录: ' + err.message);
        }
    }
}

function moveFile(oldPath, newPath, fileName) {
    let newPathWithSuffix = newPath;
    let counter = 1;

    // 检查目标文件是否已经存在，如果存在则重命名
    while (fs.existsSync(newPathWithSuffix)) {
        const parsedPath = path.parse(newPath);
        newPathWithSuffix = path.join(parsedPath.dir, `${parsedPath.name}_${counter}${parsedPath.ext}`);
        counter++;
    }

    // 移动文件
    try {
        fs.renameSync(oldPath, newPathWithSuffix);
        console.log(`移动文件: ${fileName}`);
        movedFilesCount++;
    } catch (err) {
        console.error(`无法移动文件 ${fileName}: ` + err.message);
    }
}

function deleteScriptSelf() {
    const scriptPath = __filename;
    try {
        fs.unlinkSync(scriptPath);
        console.log('程序已删除自身。');
    } catch (err) {
        console.error('无法删除程序自身: ' + err.message);
    }
}
